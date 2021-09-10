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

			std::set<String> linksfromThread = thread.extractLinks();
			spdlog::info("links: {}", String("\n").join(linksfromThread));
			links.insert(std::make_move_iterator(linksfromThread.begin()),
					std::make_move_iterator(linksfromThread.end()));

			std::set<Token> tokensfromThread = thread.extractTokens();
			spdlog::info("tokens: {}", String("\n").join(tokensfromThread));
			opTokens.insert(std::make_move_iterator(tokensfromThread.begin()),
					std::make_move_iterator(tokensfromThread.end()));

			for (const Comment& comment : comments) {
				addToIndex(comment);
			}

			threadId = thread.id;
		}

		void addToIndex(const Comment& newComment) {
			spdlog::info("indexing comment id {}", newComment.id);

			std::set<String> foundLinksInComment = newComment.extractLinks();
			spdlog::info("links in comment: {}", String("\n").join(foundLinksInComment));

			links.insert(std::make_move_iterator(foundLinksInComment.begin()),
					std::make_move_iterator(foundLinksInComment.end()));
		}

		void addToIndex(const LinkedToken& linkedToken) {
			links.insert(linkedToken.link);
			opTokens.insert(linkedToken.token);
		}

		bool inIndex(const LinkedToken& linkedToken) const {
			return tokenInIndex(linkedToken.token) or linkInIndex(linkedToken.link);
		}

		bool linkInIndex(const String& link) const {
			return links.find(link) != links.end();
		}

		bool tokenInIndex(const Token& token) const {
			return opTokens.find(token) != opTokens.end();
		}

		const String& getThreadId() const {
			return threadId;
		}
	private:
		std::set<Token> opTokens;
		std::set<String> links;
		String threadId;
};

#endif /* SRC_INDEX_H_ */
