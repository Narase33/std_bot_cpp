/*
 * SymbolIndexSearch.h
 *
 *  Created on: 13 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_LINKER_SYMBOLINDEXSEARCH_H_
#define SRC_LINKER_SYMBOLINDEXSEARCH_H_

#include "SearchBase.h"

class SymbolIndexSearch: public SearchBase {
	public:
		SymbolIndexSearch(httplib::Client& client) :
				tokenClient(client) {
		}

		std::string operator()(const Token& token) {
			if (token.type != Token::Type::std) {
				return "";
			}

			const std::vector<std::string_view> subtokens = str::split(token.content, "::");

			httplib::Result result = tokenClient.Get("/w/cpp/symbol_index");
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
			check(result->status == 200, result->reason);

			for (size_t i = 1; i < subtokens.size() - 1; i++) {
				const std::optional<std::string_view> link = findLinkToSubtoken(result->body, subtokens[i], "<code>", "</code>");
				if (!link.has_value()) {
					return "";
				}

				const std::string url(link.value());
				result = tokenClient.Get(url.c_str());
				check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
				check(result->status == 200, result->reason);
			}

			const std::optional<std::string_view> link = findLinkToSubtoken(result->body, subtokens.back(), "<tt>", "</tt>");
			if (!link.has_value()) {
				return "";
			}

			return std::string(link.value());
		}

	private:
		httplib::Client& tokenClient;
};

#endif /* SRC_LINKER_SYMBOLINDEXSEARCH_H_ */
