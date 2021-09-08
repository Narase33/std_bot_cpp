/*
 * RedditThread.h
 *
 *  Created on: 5 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_THREAD_H_
#define SRC_THREAD_H_

#include "ResponseBase.h"

#include "../../libs/json.hpp"
using Json = nlohmann::json;

struct Thread: public ResponseBase {
		Thread(const Json& json) {
			id = json["id"].get<String>();
			selftext = replaceHtmlSymbols(json["selftext"].get<String>());
			title = replaceHtmlSymbols(json["title"].get<String>());
			link = "https://www.reddit.com" + json["permalink"].get<String>();
		}

		std::set<Token> extractTokens() const {
			return extractTokensFromLine(selftext() + title());
		}

		std::set<String> extractLinks() const {
			return extractLinksFromLine(selftext() + title());
		}

		friend std::ostream& operator<<(std::ostream& o, const Thread& t) {
			return o << String("\n").concat(
					t.id,
					t.title,
					t.link,
					t.selftext);
		}

		ATTRIBUTE(String, id);
		ATTRIBUTE(String, selftext);
		ATTRIBUTE(String, title);
		ATTRIBUTE(String, link);
};

#endif /* SRC_THREAD_H_ */
