/*
 * RedditThread.h
 *
 *  Created on: 5 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_THREAD_H_
#define SRC_THREAD_H_

#include "ResponseBase.h"

#include "libs/json.hpp"
using Json = nlohmann::json;

class Thread: public ResponseBase {
	public:
		Thread(const Json& json) {
			id = json["id"].get<std::string>();
			selftext = replaceHtmlSymbols(json["selftext"].get<std::string>());
			title = replaceHtmlSymbols(json["title"].get<std::string>());
			link = "https://www.reddit.com" + json["permalink"].get<std::string>();
		}

		static bool isThreadJson(const Json& json) {
			return ResponseBase::hasKeys(json, { "id", "selftext", "title", "permalink" });
		}

		std::set<Token> extractTokens() const {
			return extractTokensFromLine(selftext + title);
		}

		std::set<std::string> extractLinks() const {
			return extractLinksFromLine(selftext + title);
		}

		std::string toString() const {
			return str::concat("\n",
					attribute("id", id),
					attribute("title", title),
					attribute("link", link),
					attribute("selftext", selftext));
		}

		friend std::ostream& operator<<(std::ostream& o, const Thread& t) {
			return o << t.toString();
		}

		std::string id;
		std::string selftext;
		std::string title;
		std::string link;
};

#endif /* SRC_THREAD_H_ */
