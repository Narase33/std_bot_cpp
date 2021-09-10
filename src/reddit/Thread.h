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

class Thread: public ResponseBase {
	public:
		Thread(const Json& json) {
			id = json["id"].get<String>();
			selftext = replaceHtmlSymbols(json["selftext"].get<String>());
			title = replaceHtmlSymbols(json["title"].get<String>());
			link = "https://www.reddit.com" + json["permalink"].get<String>();
		}

		std::set<Token> extractTokens() const {
			return extractTokensFromLine(selftext + title);
		}

		std::set<String> extractLinks() const {
			return extractLinksFromLine(selftext + title);
		}

		String toString() const {
			return String("\n").concat(
					attribute("id", id),
					attribute("title", title),
					attribute("link", link),
					attribute("selftext", selftext));
		}

		friend std::ostream& operator<<(std::ostream& o, const Thread& t) {
			return o << t.toString();
		}

		String id;
		String selftext;
		String title;
		String link;
};

#endif /* SRC_THREAD_H_ */
