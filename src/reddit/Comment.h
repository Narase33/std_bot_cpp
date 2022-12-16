/*
 * RedditComment.h
 *
 *  Created on: 4 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_COMMENT_H_
#define SRC_COMMENT_H_

#include <nlohmann/json.hpp>
using Json = nlohmann::json;

#include "ResponseBase.h"
#include "src/Tools.h"
#include "src/Token.h"

class Comment: public ResponseBase {
	public:
		Comment(const Json& json) {
			id = json["id"].get<std::string>();

			body = replaceHtmlSymbols(json["body"].get<std::string>());
			str::replace_all(body, "\\_", "_");

			author = json["author"].get<std::string>();

			created = json["created"].get<size_t>();

			fullName = json["name"].get<std::string>();

			link = "https://www.reddit.com" + json["permalink"].get<std::string>();

			const std::string parentId_entry = json["parent_id"].get<std::string>();
			parentId = parentId_entry.substr(3); // t1_[...], t3_[...]
			isTopLevelComment = str::starts_with(parentId_entry, "t3_");

			const std::string threadId_entry = json["link_id"].get<std::string>();
			threadId = threadId_entry.substr(3); // t3_[...]
		}

		static bool isCommentJson(const Json& json) {
			return ResponseBase::hasKeys(json, { "id", "body", "author", "created", "name", "permalink", "parent_id", "link_id" });
		}

		bool contains(std::string_view substr) const {
			for (const std::string_view& line : str::split(body, "\n")) {
				if (line.empty() or str::starts_with(line, ">") or str::starts_with(line, "    ") or str::starts_with(line, "\t")) {
					continue;
				}

				if (line.find(substr) != std::string_view::npos) {
					return true;
				}
			}
			return false;
		}

		std::set<Token> extractTokens() const {
			std::set<Token> tokens;
			for (const std::string_view& line : str::split(body, "\n")) {
				if (line.empty() or str::starts_with(line, ">") or str::starts_with(line, "    ") or str::starts_with(line, "\t")) {
					continue;
				}

				std::set<Token> tokensFromLine = extractTokensFromLine(std::string(line));
				tokens.insert(tokensFromLine.begin(), tokensFromLine.end());
			}
			return tokens;
		}

		std::set<std::string> extractLinks() const {
			return extractLinksFromLine(body);
		}

		std::string toString() const {
			return str::concat("\n",
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

		std::string id;
		std::string parentId;
		std::string threadId;
		std::string body;
		std::string author;
		std::string fullName;
		std::string link;
		size_t created;
		bool isTopLevelComment;
};

#endif /* SRC_COMMENT_H_ */
