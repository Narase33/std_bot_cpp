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
			SearchBase(client) {
		}

		std::string operator()(const Token& token) {
			if (token.type != Token::Type::header) {
				return "";
			}


			Page* page = getPage("/w/cpp/header");
			const std::string transformedHtmlPage = replaceHtmlSymbols(page->content);
			const std::optional<std::string_view> link = findLinkToSubtoken(transformedHtmlPage, token.content, "<tt>", "</tt>");
			if (!link.has_value()) {
				return "";
			}

			return std::string(link.value());
		}
};

#endif /* SRC_LINKER_HEADERSEARCH_H_ */
