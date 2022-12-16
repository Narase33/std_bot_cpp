#include <sstream>
#include <fstream>
#include <memory>
#include <csignal>

#include "reddit/Reddit.h"
#include "Index.h"
#include "Token.h"
#include "linker/Linker.h"

Reddit* reddit;
Cache<Index> threadCache{ std::chrono::hours(24 * 7) };
std::set<std::string> ignoredUsers;

volatile std::sig_atomic_t receivedSignal = 0;
std::mutex signalMutex;
std::thread signalThread;

const std::string signature = "\n\n---\n\n^(Last update: 16.12.22. Last Change: Search improvements)[Repo](https://github.com/Narase33/std_bot_cpp)";

Index* getIndex(const std::string& threadId) {
	Index* index = threadCache.find([&](const Index& i) {
		return i.getThreadId() == threadId;
	});

	if (index == nullptr) {
		const auto& [thread, commentList] = reddit->requestThread(threadId);
		Index newIndex(thread, commentList);
		index = threadCache.add(newIndex);
	}

	return index;
}

bool allLinksKnownInThread(const std::set<LinkedToken>& linkedTokens, const std::string& threadId) {
	Index* index = getIndex(threadId);
	return std::all_of(linkedTokens.begin(), linkedTokens.end(), [&](const LinkedToken& t) {
		return index->inIndex(t);
	});
}

std::string replyMessage(const std::set<LinkedToken>& linkedTokens) {
	std::string outMessage = "Unlinked STL entries:";
	for (const LinkedToken& linkedToken : linkedTokens) {
		outMessage += " ";
		outMessage += linkedToken.toLink();
	}
	return outMessage + signature;
}

void configureLogger() {
	const auto logLevel = spdlog::level::trace;
	const char* logPattern = "%Y.%m.%d %H:%M:%S %l | %v";

	std::vector<spdlog::sink_ptr> sinks;

	//auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
	//console_sink->set_level(logLevel);
	//console_sink->set_pattern(logPattern);
	//sinks.push_back(console_sink);

	auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/log.txt", 0, 0, false, 30);
	file_sink->set_level(logLevel);
	file_sink->set_pattern(logPattern);
	sinks.push_back(file_sink);

	auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());

	spdlog::set_default_logger(logger);
	spdlog::set_level(logLevel);
	spdlog::flush_on(logLevel);
}


void loadData() {
	std::ifstream ignoredUsersFile("data/ignored_users.txt");
	std::string line;
	while (std::getline(ignoredUsersFile, line)) {
		ignoredUsers.insert(line);
	}
}

void saveData() {
	std::ofstream ignoredUsersFile("data/ignored_users.txt");
	for (const std::string& ignoredUser : ignoredUsers) {
		ignoredUsersFile << ignoredUser << "\n";
	}
}

bool isReplyAllowed(const Comment& comment) {
	bool replyAllowed = true;

	if (!comment.isTopLevelComment) {
		replyAllowed = false;
		spdlog::info("Comment not top level");
	}

	if (ignoredUsers.find(comment.author) != ignoredUsers.end()) {
		replyAllowed = false;
		spdlog::info("Author in ignore list");
	}

	if (comment.contains("!std") and !(comment.contains("!std ignore_me") or comment.contains("!std follow_me"))) {
		replyAllowed = true;
		spdlog::info("Forced comment");
	}

	return replyAllowed;
}

void lookForBotCommands(const Comment& comment) {
	if (comment.contains("!std ignore_me")) {
		ignoredUsers.insert(comment.author);
		saveData();
		reddit->comment(comment.fullName, "Alright, not following you anymore" + signature);
	}

	if (comment.contains("!std follow_me")) {
		ignoredUsers.erase(comment.author);
		saveData();
		reddit->comment(comment.fullName, "Happy to follow you again" + signature);
	}
}

void signalHandler(int signal) {
	receivedSignal = signal;
	signalMutex.unlock();
}

void setupSignalHandler() {
	signalMutex.lock();

	for (auto sig : { SIGABRT, SIGFPE, SIGINT, SIGSEGV, SIGTERM }) {
		std::signal(sig, signalHandler);
	}

	signalThread = std::thread([&]() {
		std::unique_lock<std::mutex> lock(signalMutex);
		spdlog::critical("Received signal: {}", receivedSignal);
		spdlog::shutdown();
		std::exit(-1);
	});
}

void debugComment(const char* fullName) {
	Linker linker;

	const Comment comment = reddit->requestComment(fullName);
	std::cout << comment << std::endl;

	// getIndex(comment.threadId)->addToIndex(comment);
	lookForBotCommands(comment);

	const std::set<Token> tokens = comment.extractTokens();
	spdlog::info("tokens in comment: {}", str::join("\n", tokens));

	const std::set<LinkedToken> linkedTokens = linker.getLinkedTokens(tokens);
	spdlog::info("linked tokens: {}", str::join("\n", linkedTokens));

	if (!allLinksKnownInThread(linkedTokens, comment.threadId)) {
		spdlog::info("No tokens to link");
		spdlog::info("{}\n\n\n\n\n", std::string(40, '-'));
		return;
	}

	std::string reply = replyMessage(linkedTokens);
	spdlog::info("Possible reply:\n{}", reply);

	if (isReplyAllowed(comment)) {
		// reddit.comment(comment.fullName, std::move(reply));
		spdlog::info("Reply sent");
	} else {
		spdlog::info("Reply canceled");
	}

	spdlog::info("{}\n\n\n\n\n", std::string(40, '-'));
}

void simpleTest(Linker& linker, const std::string& token, std::string_view link) {
	const LinkedToken* linkedToken = linker.getLinkedToken(Token(token));
	check(linkedToken->link == link, "'" + token + "' link not found");
}

void simpleTests() {
	try {
		Linker linker;

		simpleTest(linker, "std::vector", "https://en.cppreference.com/w/cpp/container/vector");
		simpleTest(linker, "std::vector<int>", "https://en.cppreference.com/w/cpp/container/vector");

		simpleTest(linker, "std::vector::push_back", "https://en.cppreference.com/w/cpp/container/vector/push_back");
		simpleTest(linker, "std::vector::push_back()", "https://en.cppreference.com/w/cpp/container/vector/push_back");
		simpleTest(linker, "std::vector<int>::push_back", "https://en.cppreference.com/w/cpp/container/vector/push_back");
		simpleTest(linker, "std::vector<int>::push_back()", "https://en.cppreference.com/w/cpp/container/vector/push_back");

		simpleTest(linker, "std::numeric_limits<int>::min()", "https://en.cppreference.com/w/cpp/types/numeric_limits/min");

		simpleTest(linker, "std::chrono", "https://en.cppreference.com/w/cpp/chrono");
		simpleTest(linker, "std::chrono::seconds", "https://en.cppreference.com/w/cpp/chrono/duration");
		simpleTest(linker, "std::chrono::duration", "https://en.cppreference.com/mwiki/index.php?title=Special%3ASearch&search=std%3A%3Achrono%3A%3Aduration");

		simpleTest(linker, "<bitset>", "https://en.cppreference.com/w/cpp/header/bitset");
	} catch (std::runtime_error& e) {
		spdlog::critical(e.what());
		std::exit(-1);
	}
}

int main() {
	reddit = new Reddit();

	configureLogger();
	setupSignalHandler();

	simpleTests();

	//debugComment("t1_hdg7ykk");

	loadData();

	spdlog::info("Starting bot");

	Linker linker;
	while (true) {
		try {
			const std::vector<Comment> comments = reddit->requestNewComments();

			for (const Comment& comment : comments) {
				spdlog::info("{}\n\n\n\n\n", std::string(40, '-'));

				spdlog::info("new comment:\n{}", comment.toString());
				Index* index = getIndex(comment.threadId);

				index->addToIndex(comment);
				lookForBotCommands(comment);

				const std::set<Token> tokens = comment.extractTokens();
				if (tokens.size() == 0) {
					spdlog::info("no tokens in comment");
					continue;
				}

				spdlog::info("tokens in comment: {}", str::join("\n", tokens));

				const std::set<LinkedToken> linkedTokens = linker.getLinkedTokens(tokens);
				spdlog::info("linked tokens: {}", str::join("\n", linkedTokens));

				if (allLinksKnownInThread(linkedTokens, comment.threadId)) {
					spdlog::info("No tokens to link");
					continue;
				}

				std::string reply = replyMessage(linkedTokens);
				spdlog::info("Possible reply:\n{}", reply);

				if (isReplyAllowed(comment)) {
					reddit->comment(comment.fullName, std::move(reply));
					spdlog::info("Reply sent");

					for (const LinkedToken& t : linkedTokens) {
						index->addToIndex(t);
					}
				} else {
					spdlog::info("Reply canceled");
				}
			}
		} catch (std::exception& e) {
			spdlog::error("{}\n", e.what());
		} catch (...) {
			spdlog::critical("Catched unknown error!\n");
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}
