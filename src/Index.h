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

			std::vector<LinkedToken> linksfromThread = thread.extractLinks();
			spdlog::info("links: {}", str::join("\n", linksfromThread));
			moveInsert(std::move(linksfromThread), links);

			std::set<Token> tokensfromThread = thread.extractTokens();
			spdlog::info("tokens: {}", str::join("\n", tokensfromThread));
			moveInsert(std::move(tokensfromThread), opTokens);

			for (const Comment& comment : comments) {
				addToIndex(comment);
			}

			threadId = thread.id;
		}

		void addToIndex(const Comment& newComment) {
			spdlog::info("indexing comment id {}", newComment.id);

			std::vector<LinkedToken> foundLinksInComment = newComment.extractLinks();
			spdlog::info("links in comment: {}", str::join("n", foundLinksInComment));

			moveInsert(std::move(foundLinksInComment), links);
		}

		void addToIndex(const LinkedToken& linkedToken) {
			links.push_back(linkedToken);
			opTokens.insert(linkedToken.token).second;
		}

		bool inIndex(const Token& token) const {
			return tokenInIndex(token) or linkInIndex(token);
		}

		bool linkInIndex(const Token& token) const {
			return std::find_if(links.begin(), links.end(), [&](const LinkedToken& lt) {
				return lt.token == token;
			}) != links.end();
		}

		bool tokenInIndex(const Token& token) const {
			return opTokens.find(token) != opTokens.end();
		}

		const std::string& getThreadId() const {
			return threadId;
		}
	private:
		std::set<Token> opTokens;
		std::vector<LinkedToken> links;
		std::string threadId;
};

#endif /* SRC_INDEX_H_ */
