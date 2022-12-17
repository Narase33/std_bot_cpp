/*
 * SymbolIndexSearch.h
 *
 *  Created on: 13 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_LINKER_SYMBOLINDEXSEARCH_H_
#define SRC_LINKER_SYMBOLINDEXSEARCH_H_

#include "SearchBase.h"

class SymbolIndexSearch : public SearchBase {
public:
	SymbolIndexSearch(httplib::Client& client) :
		SearchBase(client) {
	}

	std::string operator()(const Token& token) {
		if (token.type != Token::Type::std) {
			return "";
		}

		std::vector<std::string_view> subtokens = str::split(token.content, "::");
		for (std::string_view& v : subtokens) {
			const size_t start_template = v.find('<');
			if (start_template != std::string_view::npos) {
				v.remove_suffix(v.size() - start_template);
			}

			const size_t start_parenthesis = v.find('(');
			if (start_parenthesis != std::string_view::npos) {
				v.remove_suffix(v.size() - start_parenthesis);
			}
		}

		Page* page = getPage("/w/cpp/symbol_index");
		for (size_t i = 1; i < subtokens.size(); i++) {
			std::optional<std::string_view> link;

			if (!link.has_value()) {
				link = findLinkToSubtoken(page->content, subtokens[i], "<span>", "</span>");
			}

			if (!link.has_value()) {
				link = findLinkToSubtoken(page->content, subtokens[i], "<code>", "</code>");
			}

			if (!link.has_value()) {
				link = findLinkToSubtoken(page->content, subtokens[i], "<tt>", "</tt>");
			}

			if (!link.has_value()) {
				return "";
			}


			if (i == subtokens.size() - 1) {
				return std::string(link.value());
			}

			page = getPage(std::string(link.value()));
		}

		return "";
	}
};

#endif /* SRC_LINKER_SYMBOLINDEXSEARCH_H_ */
