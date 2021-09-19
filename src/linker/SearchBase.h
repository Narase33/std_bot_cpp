/*
 * SearchBase.h
 *
 *  Created on: 13 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_LINKER_SEARCHBASE_H_
#define SRC_LINKER_SEARCHBASE_H_

#include <vector>

#include "src/Tools.h"
#include "src/Token.h"

class SearchBase {

	protected:
		bool isSubtoken(std::string_view str, std::string_view subtoken) const {
			if (str::ends_with(str, "<>")) {
				str.remove_suffix(2);
			}

			return str == subtoken;
		}

		std::optional<std::string_view> findLinkToSubtoken(const std::string& htmlPage, std::string_view subtoken, const std::string& capsulationBegin, const std::string& capsulationEnd) const {
			auto [link_element, link_index] = snip(htmlPage, "<a href", "</a>");
			while (!link_element.empty()) {
				const auto [token_element, token_index] = snip_between(link_element, capsulationBegin, capsulationEnd);

				if (isSubtoken(token_element, subtoken)) {
					const auto [url, url_index] = snip_between(link_element, "\"", "\"");
					return url;
				}

				std::tie(link_element, link_index) = snip(htmlPage, "<a href", "</a>", link_index);
			}
			return {};
		}
};

#endif /* SRC_LINKER_SEARCHBASE_H_ */
