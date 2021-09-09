#include <sstream>
#include <fstream>
#include <memory>

#include "reddit/Reddit.h"
#include "Index.h"
#include "Token.h"
#include "Linker.h"

Reddit reddit;
Cache<Index> threadCache { std::chrono::hours(24 * 7) };

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
	const String signature = "\n\n---\n\n^(Last update: 07.09.21. Last Change: Bot is now C++!)[readme](https:://github.com/Narase33/std_bot/blob/main/README.md)";

	String outMessage = "Unlinked STL entries:";
	for (const LinkedToken& linkedToken : linkedTokens) {
		outMessage += " " + linkedToken.toLink();
	}
	return outMessage + signature;
}

void startingTests(bool b) {
	if (!b) {
		return;
	}

	std::set<Token> tokens;
	tokens.insert(Token("std::vector"));
	tokens.insert(Token("std::fghjkl"));
	tokens.insert(Token("std::chrono::seconds"));
	tokens.insert(Token("std::chrono"));

	const std::set<LinkedToken> linkedTokens = linker::getLinkedTokens(tokens);
	check(linkedTokens.size() == 2, std::to_string(linkedTokens.size()));

	const auto pos_vector = std::find_if(linkedTokens.begin(), linkedTokens.end(), [](const LinkedToken& lt) {
		return lt.token.content == "std::vector";
	});
	check(pos_vector != linkedTokens.end(), "std::vector not in list");
	check(!pos_vector->link.empty(), "std::vector not linked");

	const auto pos_chrono_seconds = std::find_if(linkedTokens.begin(), linkedTokens.end(), [](const LinkedToken& lt) {
		return lt.token.content == "std::chrono::seconds";
	});
	check(pos_chrono_seconds != linkedTokens.end(), "std::chrono::seconds not in list");
	check(!pos_chrono_seconds->link.empty(), "std::chrono::seconds not linked");
}

void configureLogger() {
	const auto logLevel = spdlog::level::trace;
	const char* logPattern = "[%Y.%m.%d %H:%M:%S] [%l] %v";

	auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
	console_sink->set_level(logLevel);
	console_sink->set_pattern(logPattern);

	auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_st>("logs/log.txt", 0, 0);
	file_sink->set_level(logLevel);
	file_sink->set_pattern(logPattern);

	std::vector<spdlog::sink_ptr> sinks { console_sink, file_sink };
	auto logger = std::make_shared<spdlog::logger>("logger", sinks.begin(), sinks.end());

	spdlog::register_logger(logger);
	spdlog::set_level(logLevel);
}

int main() {
	configureLogger();

	spdlog::info("Starting bot");

	startingTests(true);

	while (true) {
		try {
			const std::vector<Comment> comments = reddit.requestNewComments();
			for (const Comment& comment : comments) {
				spdlog::info("new comment:\n{}", comment.toString());

				const std::set<Token> tokens = comment.extractTokens();
				spdlog::info("tokens in comment: {}", String("\n").join(tokens));

				const std::set<LinkedToken> linkedTokens = linker::getLinkedTokens(tokens);
				spdlog::info("linked tokens: {}", String("\n").join(linkedTokens));

				if (!addLinkedTokens(linkedTokens, comment.threadId())) {
					spdlog::info("No tokens to link");
					spdlog::info("\n{}\n\n", std::string(40, '-'));
					continue;
				}

				String reply = replyMessage(linkedTokens);
				std::cout << reply << std::endl;
				// reddit.comment(comment.fullName, std::move(reply));

				spdlog::info("\n{}\n\n", std::string(40, '-'));
			}
		} catch (std::exception& e) {
			spdlog::error(e.what());
		} catch (...) {
			spdlog::error("Catched unknown error!");
		}
		std::this_thread::sleep_for(std::chrono::seconds(2));
	}
}