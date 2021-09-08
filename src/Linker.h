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

namespace linker {
	Cache<LinkedToken> tokenCache(std::chrono::hours(24 * 7));

	inline bool isSubtoken(String str, const String& subtoken) {
		size_t i = 0;
		for (; i < str.length(); i++) {
			if (!(std::isalnum(str[i]) or (str[i] == '_'))) {
				break;
			}
		}

		str = str.substr(0, i);
		return str == subtoken;
	}

	inline String findLinkToSubtoken(const String& htmlPage, const String& subtoken, const String& encapsulation) {
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
		return "";
	}

	inline std::optional<LinkedToken> addLink(const Token& token) {
		const std::vector<String> subtokens = token.content.split("::");

		httplib::Client tokenClient("https://en.cppreference.com");

		httplib::Result requestResult = tokenClient.Get("/w/cpp/symbol_index");
		check(requestResult, "Error state: ", static_cast<int>(requestResult.error()), HERE);
		check(requestResult->status == 200, requestResult->reason);

		for (size_t i = 1; i < subtokens.size() - 1; i++) {
			const String link = findLinkToSubtoken(requestResult->body, subtokens[i], "code");
			if (link.empty()) {
				return {};
			}

			requestResult = tokenClient.Get(link.c_str());
			check(requestResult, "Error state: ", static_cast<int>(requestResult.error()), HERE);
			check(requestResult->status == 200, requestResult->reason);
		}

		const String link = findLinkToSubtoken(requestResult->body, subtokens.back(), "tt");
		if (link.empty()) {
			return {};
		}

		LinkedToken linkedToken(token, "https://en.cppreference.com" + link);
		return *tokenCache.add(linkedToken);
	}

	inline std::optional<LinkedToken> getLinkedToken(const Token& token) {
		LinkedToken* entry = tokenCache.find([&](const LinkedToken& linkedToken) {
			return linkedToken == token;
		});

		return entry != nullptr ? *entry : addLink(token);
	}

	inline std::set<LinkedToken> getLinkedTokens(const std::set<Token>& tokens) {
		std::set < LinkedToken > linkedTokens;

		for (const Token& token : tokens) {
			std::optional<LinkedToken> linkedToken = getLinkedToken(token);
			if (linkedToken.has_value()) {
				linkedTokens.insert(std::move(linkedToken.value()));
			}
		}

		return linkedTokens;
	}
}

#endif /* SRC_LINKER_H_ */
