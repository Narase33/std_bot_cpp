/*
 * Linker.h
 *
 *  Created on: 7 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_LINKER_H_
#define SRC_LINKER_H_

#include <set>

#include "Tools.h"

class Linker {
	public:
		std::optional<LinkedToken> getLinkedToken(const Token& token) {
			LinkedToken* entry = tokenCache.find([&](const LinkedToken& linkedToken) {
				return linkedToken == token;
			});

			return entry != nullptr ? *entry : searchForLink(token);
		}

		std::set<LinkedToken> getLinkedTokens(const std::set<Token>& tokens) {
			std::set<LinkedToken> linkedTokens;

			for (const Token& token : tokens) {
				std::optional<LinkedToken> linkedToken = getLinkedToken(token);
				if (linkedToken.has_value()) {
					linkedTokens.insert(std::move(linkedToken.value()));
				}
			}

			return linkedTokens;
		}

	private:
		Cache<LinkedToken> tokenCache { std::chrono::hours(24 * 7) };
		httplib::Client tokenClient { "https://en.cppreference.com" };

		bool isSubtoken(String str, const String& subtoken) const {
			size_t i = 0;
			for (; i < str.length(); i++) {
				if (!(std::isalnum(str[i]) or (str[i] == '_'))) {
					break;
				}
			}

			str = str.substr(0, i);
			return str == subtoken;
		}

		std::optional<String> findLinkToSubtoken(const String& htmlPage, const String& subtoken, const String& encapsulation) const {
			const String capsulationBegin = "<" + encapsulation + ">";
			const String capsulationEnd = "</" + encapsulation + ">";

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

		std::optional<LinkedToken> searchForLink(const Token& token) {
			const std::vector<String> subtokens = token.content.split("::");

			httplib::Result result = tokenClient.Get("/w/cpp/symbol_index");
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", HERE);
			check(result->status == 200, result->reason);

			for (size_t i = 1; i < subtokens.size() - 1; i++) {
				const std::optional<String> link = findLinkToSubtoken(result->body, subtokens[i], "code");
				if (!link.has_value()) {
					return {};
				}

				result = tokenClient.Get(link->c_str());
				check(result, "Error state: ", httplib::to_string(result.error()), "\n", HERE);
				check(result->status == 200, result->reason);
			}

			const std::optional<String> link = findLinkToSubtoken(result->body, subtokens.back(), "tt");
			if (!link.has_value()) {
				return {};
			}

			LinkedToken linkedToken(token, "https://en.cppreference.com" + link.value());
			return *tokenCache.add(linkedToken);
		}
};

#endif /* SRC_LINKER_H_ */
