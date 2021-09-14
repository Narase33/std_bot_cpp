/*
 * Linker.h
 *
 *  Created on: 7 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_LINKER_H_
#define SRC_LINKER_H_

#include <set>

#include "SymbolIndexSearch.h"
#include "EngineSearch.h"

class Linker {
	public:
		LinkedToken* getLinkedToken(const Token& token) {
			LinkedToken* entry = tokenCache.find([&](const LinkedToken& linkedToken) {
				return linkedToken == token;
			});

			return entry != nullptr ? entry : searchForLink(token);
		}

		std::set<LinkedToken> getLinkedTokens(const std::set<Token>& tokens) {
			std::set<LinkedToken> linkedTokens;

			for (const Token& token : tokens) {
				LinkedToken* linkedToken = getLinkedToken(token);
				if (linkedToken != nullptr) {
					linkedTokens.insert(*linkedToken);
				}
			}

			return linkedTokens;
		}

	private:
		Cache<LinkedToken> tokenCache { std::chrono::hours(24 * 7) };
		httplib::Client tokenClient { "https://en.cppreference.com" };
		SymbolIndexSearch symbolSearch { tokenClient };
		EngineSearch engineSearch { tokenClient };
		const std::map<std::string, std::string> namespaceLinks = {
				{ "std::chrono", "https://en.cppreference.com/w/cpp/chrono" },
				{ "std::filesystem", "https://en.cppreference.com/w/cpp/filesystem" },
				{ "std::ranges", "https://en.cppreference.com/w/cpp/ranges" }
		};

		LinkedToken* searchForLink(const Token& token) {
			std::string link;

			const auto namespaceLink = namespaceLinks.find(token.content);
			if (namespaceLink != namespaceLinks.end()) {
				link = namespaceLink->second;
			}

			if (link.empty()) {
				link = symbolSearch(token);
			}

			if (link.empty()) {
				link = engineSearch(token);
			}

			if (link.empty()) {
				return nullptr;
			}

			LinkedToken linkedToken(token, "https://en.cppreference.com" + link);
			return tokenCache.add(std::move(linkedToken));
		}
};

#endif /* SRC_LINKER_H_ */
