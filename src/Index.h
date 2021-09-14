/*
 * Index.h
 *
 *  Created on: 5 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_INDEX_H_
#define SRC_INDEX_H_

#include <set>

#include "Cache.h"
#include "reddit/Comment.h"
#include "reddit/Thread.h"
#include "Token.h"

class Index {
	public:
		Index(const Thread& thread, const std::vector<Comment>& comments) {
			spdlog::info("indexing new thread id {}", thread.id);

			std::set<std::string> linksfromThread = thread.extractLinks();
			spdlog::info("links: {}", str_tools::join("\n", linksfromThread));
			links.insert(std::make_move_iterator(linksfromThread.begin()),
					std::make_move_iterator(linksfromThread.end()));

			std::set<Token> tokensfromThread = thread.extractTokens();
			spdlog::info("tokens: {}", str_tools::join("\n", tokensfromThread));
			opTokens.insert(std::make_move_iterator(tokensfromThread.begin()),
					std::make_move_iterator(tokensfromThread.end()));

			for (const Comment& comment : comments) {
				addToIndex(comment);
			}

			threadId = thread.id;
		}

		void addToIndex(const Comment& newComment) {
			spdlog::info("indexing comment id {}", newComment.id);

			std::set<std::string> foundLinksInComment = newComment.extractLinks();
			spdlog::info("links in comment: {}", str_tools::join("n", foundLinksInComment));

			links.insert(std::make_move_iterator(foundLinksInComment.begin()),
					std::make_move_iterator(foundLinksInComment.end()));
		}

		bool addToIndex(const LinkedToken& linkedToken) {
			return links.insert(linkedToken.link).second | opTokens.insert(linkedToken.token).second;
		}

		bool inIndex(const LinkedToken& linkedToken) const {
			return tokenInIndex(linkedToken.token) or linkInIndex(linkedToken.link);
		}

		bool linkInIndex(const std::string& link) const {
			return links.contains(link);
		}

		bool tokenInIndex(const Token& token) const {
			return opTokens.contains(token);
		}

		const std::string& getThreadId() const {
			return threadId;
		}
	private:
		std::set<Token> opTokens;
		std::set<std::string> links;
		std::string threadId;
};

#endif /* SRC_INDEX_H_ */
