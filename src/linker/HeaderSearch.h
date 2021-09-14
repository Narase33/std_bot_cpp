/*
 * HeaderSearch.h
 *
 *  Created on: 14 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_LINKER_HEADERSEARCH_H_
#define SRC_LINKER_HEADERSEARCH_H_

#include "SearchBase.h"

class HeaderSearch: public SearchBase {
	public:
		HeaderSearch(httplib::Client& client) :
				tokenClient(client) {
		}

		std::string operator()(const Token& token) {
			if (token.type != Token::Type::header) {
				return "";
			}

			httplib::Result result = tokenClient.Get("/w/cpp/header");
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
			check(result->status == 200, result->reason);

			const std::string transformedHtmlPage = replaceHtmlSymbols(result->body);
			const std::optional<std::string_view> link = findLinkToSubtoken(transformedHtmlPage, token.content, "<tt>", "</tt>");
			if (!link.has_value()) {
				return "";
			}

			return std::string(link.value());
		}

	private:
		httplib::Client& tokenClient;
};

#endif /* SRC_LINKER_HEADERSEARCH_H_ */
