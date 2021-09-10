/*
 * RedditComment.h
 *
 *  Created on: 4 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_COMMENT_H_
#define SRC_COMMENT_H_

#include "../../libs/json.hpp"
using Json = nlohmann::json;

#include "ResponseBase.h"
#include "../Tools.h"
#include "../Token.h"

class Comment: public ResponseBase {
	public:
		Comment(const Json& json) {
			id = json["id"].get<String>();
			body = replaceHtmlSymbols(json["body"].get<String>());
			author = json["author"].get<String>();
			created = json["created"].get<size_t>();
			fullName = json["link_id"].get<String>();
			link = "https://www.reddit.com" + json["permalink"].get<String>();

			const String parentId_entry = json["parent_id"].get<String>();
			parentId = parentId_entry.substr(3); // t1_[...], t3_[...]
			isTopLevelComment = parentId_entry.starts_with("t3_");

			const String threadId_entry = json["link_id"].get<String>();
			threadId = threadId_entry.substr(3); // t3_[...]
		}

		std::set<Token> extractTokens() const {
			std::set<Token> tokens;
			for (const String& line : body.split('\n')) {
				if (line.starts_with('>') or line.starts_with("    ")) {
					continue;
				}

				std::set<Token> tokensFromLine = extractTokensFromLine(line);
				tokens.insert(tokensFromLine.begin(), tokensFromLine.end());
			}
			return tokens;
		}

		std::set<String> extractLinks() const {
			return extractLinksFromLine(body);
		}

		String toString() const {
			return String("\n").concat(
					attribute("author", author),
					attribute("isTopLevelComment", isTopLevelComment),
					attribute("id", id),
					attribute("parentId", parentId),
					attribute("threadId", threadId),
					attribute("link", link),
					attribute("created", created),
					attribute("body", body));
		}

		friend std::ostream& operator<<(std::ostream& o, const Comment& c) {
			return o << c.toString();
		}

		String id;
		String parentId;
		String threadId;
		String body;
		String author;
		String fullName;
		String link;
		size_t created;
		bool isTopLevelComment;
};

#endif /* SRC_COMMENT_H_ */
