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
#include "HeaderSearch.h"

class Linker {
	public:
		LinkedToken* getLinkedToken(const Token& token) {
			LinkedToken* entry = tokenCache.find([&](const LinkedToken& linkedToken) {
				return linkedToken == token;
			});

			if (entry != nullptr) {
				spdlog::info("Found '{}' in cache", token.content);
			} else {
				spdlog::info("'{}' not in cache, starting search", token.content);
				entry = searchForLink(token);
			}
			return entry;
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
		HeaderSearch headerSearch { tokenClient };
		const std::map<std::string, std::string> namespaceLinks = {
				{ "std::chrono", "/w/cpp/chrono" },
				{ "std::filesystem", "/w/cpp/filesystem" },
				{ "std::ranges", "/w/cpp/ranges" }
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
				link = headerSearch(token);
			}

			if (link.empty()) {
				return nullptr;
			}

			LinkedToken linkedToken(token, "https://en.cppreference.com" + link);
			return tokenCache.add(std::move(linkedToken));
		}
};

#endif /* SRC_LINKER_H_ */
