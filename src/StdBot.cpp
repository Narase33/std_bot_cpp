#include <sstream>
#include <fstream>
#include <memory>

#include "reddit/Reddit.h"
#include "Index.h"
#include "Token.h"
#include "Linker.h"

using namespace std::string_literals;

Reddit reddit;
Cache<Index> threadCache { std::chrono::hours(24 * 7) };
std::set<String> ignoredUsers;

const String signature = "\n\n---\n\n^(Last update: 10.09.21. Last Change: Bot is now C++!)[Repo](https:://github.com/Narase33/std_bot_cpp)";

Index* getIndex(const String& threadId) {
	Index* index = threadCache.find([&](const Index& i) {
		return i.getThreadId() == threadId;
	});

	if (index == nullptr) {
		const auto& [thread, commentList] = reddit.requestThread(threadId);
		Index newIndex(thread, commentList);
		index = threadCache.add(newIndex);
	}

	return index;
}

bool addLinkedTokens(const std::set<LinkedToken>& linkedTokens, const String& threadId) {
	Index* index = getIndex(threadId);

	bool allTokensLinked = true;
	for (const LinkedToken& linkedToken : linkedTokens) {
		allTokensLinked &= index->inIndex(linkedToken);
		index->addToIndex(linkedToken);
	}

	return !allTokensLinked;
}

String replyMessage(const std::set<LinkedToken>& linkedTokens) {
	String outMessage = "Unlinked STL entries:";
	for (const LinkedToken& linkedToken : linkedTokens) {
		outMessage += " " + linkedToken.toLink();
	}
	return outMessage + signature;
}

void configureLogger() {
	const auto logLevel = spdlog::level::trace;
	const char* logPattern = "%Y.%m.%d %H:%M:%S %l | %v";

	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
	console_sink->set_level(logLevel);
	console_sink->set_pattern(logPattern);

	auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/log.txt", 0, 0);
	file_sink->set_level(logLevel);
	file_sink->set_pattern(logPattern);

	std::vector<spdlog::sink_ptr> sinks { console_sink, file_sink };
	auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());

	spdlog::set_default_logger(logger);
	spdlog::set_level(logLevel);
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
	for (const String& ignoredUser : ignoredUsers) {
		ignoredUsersFile << ignoredUser << "\n";
	}
}

bool isReplyAllowed(const Comment& comment) {
	bool replyAllowed = comment.isTopLevelComment;

	if (ignoredUsers.find(comment.author) != ignoredUsers.end()) {
		replyAllowed = false;
	}

	if (comment.body.contains("!std")) {
		replyAllowed = true;
	}

	return replyAllowed;
}

void lookForBotCommands(const Comment& comment) {
	if (comment.body.contains("!std ignore_me")) {
		ignoredUsers.insert(comment.author);
		saveData();
		reddit.comment(comment.fullName, "Alright, not following you anymore" + signature);
	}

	if (comment.body.contains("!std follow_me")) {
		ignoredUsers.erase(comment.author);
		saveData();
		reddit.comment(comment.fullName, "Happy to follow you again" + signature);
	}
}

int main() {
	configureLogger();
	loadData();

	spdlog::info("Starting bot");

	Linker linker;

	while (true) {
		try {
			const std::vector<Comment> comments = reddit.requestNewComments();
			for (const Comment& comment : comments) {
				spdlog::info("new comment:\n{}", comment.toString());

				getIndex(comment.threadId)->addToIndex(comment);
				lookForBotCommands(comment);

				const std::set<Token> tokens = comment.extractTokens();
				spdlog::info("tokens in comment: {}", String("\n").join(tokens));

				const std::set<LinkedToken> linkedTokens = linker.getLinkedTokens(tokens);
				spdlog::info("linked tokens: {}", String("\n").join(linkedTokens));

				if (!addLinkedTokens(linkedTokens, comment.threadId)) {
					spdlog::info("No tokens to link");
					spdlog::info("{}\n\n\n\n\n", std::string(40, '-'));
					continue;
				}

				String reply = replyMessage(linkedTokens);
				spdlog::info("Possible reply:\n{}", reply);

				if (isReplyAllowed(comment)) {
					reddit.comment(comment.fullName, std::move(reply));
					spdlog::info("Reply sent");
				} else {
					spdlog::info("Reply canceled");
				}

				spdlog::info("{}\n\n\n\n\n", std::string(40, '-'));
			}
		} catch (std::exception& e) {
			spdlog::error(e.what());
		} catch (...) {
			spdlog::error("Catched unknown error!");
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}
