/*
 * SearchBase.h
 *
 *  Created on: 13 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_LINKER_SEARCHBASE_H_
#define SRC_LINKER_SEARCHBASE_H_

#include <vector>
#include <optional>

#include <httplib/httplib.h>

#include "src/Cache.h"
#include "src/Tools.h"
#include "src/Token.h"

class SearchBase {
public:
	SearchBase(httplib::Client& client) :
		tokenClient(client) {
	}

protected:
	httplib::Client& tokenClient;

	struct Page {
		std::string url;
		std::string content;
	};

	Cache<Page> _cache{ std::chrono::hours(24 * 7) };

	Page* getPage(std::string url) {
		Page* page = _cache.tryGet([url](const Page& p) {
			return p.url == url;
		});


		if (page != nullptr) {
			spdlog::info("Found '{}' in cache", url);
		} else {
			spdlog::info("'{}' not in cache, making request", url);

			httplib::Result result = tokenClient.Get(url);
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
			check((result->status == 200) or (result->status == 302), result->reason);

			page = _cache.add({ std::move(url), std::move(result->body) });
		}

		return page;
	}

	void removeSuffix(std::string_view& strv, std::string_view suffix) const {
		if (str::ends_with(strv, suffix)) {
			strv.remove_suffix(suffix.length());
		}
	}

	bool isSubtoken(std::string_view strv, std::string_view subtoken) const {
		removeSuffix(strv, "<>");
		removeSuffix(strv, "&lt;&gt;");

		return strv == subtoken;
	}

	std::optional<std::string_view> findLinkToSubtoken(const std::string& htmlPage, std::string_view subtoken, const std::string& capsulationBegin, const std::string& capsulationEnd) const {
		const std::string_view link_token_begin = "<a href=\"";
		const std::string_view link_token_end = "\"";

		const std::string tokenWithCapsulationbegin = capsulationBegin + std::string(subtoken);

		size_t capsulationBegin_pos = htmlPage.find(tokenWithCapsulationbegin);
		while (capsulationBegin_pos != std::string::npos) {
			std::string_view token_element = std::string_view(htmlPage);
			token_element.remove_prefix(capsulationBegin_pos + capsulationBegin.length());

			const size_t capsulationEnd_pos = token_element.find(capsulationEnd);
			if (capsulationEnd_pos == std::string::npos) {
				return {};
			}
			token_element.remove_suffix(token_element.length() - capsulationEnd_pos);

			if (isSubtoken(token_element, subtoken)) {
				std::string_view link = std::string_view(htmlPage);
				const size_t link_token_begin_pos = link.rfind(link_token_begin, capsulationBegin_pos);
				if (link_token_begin_pos == std::string::npos) {
					return {};
				}
				link.remove_prefix(link_token_begin_pos + link_token_begin.length());


				const size_t link_token_end_pos = link.find(link_token_end);
				if (link_token_end_pos == std::string::npos) {
					return {};
				}
				link.remove_suffix(link.length() - link_token_end_pos);

				return link;
			}

			capsulationBegin_pos = htmlPage.find(tokenWithCapsulationbegin, capsulationBegin_pos + tokenWithCapsulationbegin.length());
		}
		return {};
	}
};

#endif /* SRC_LINKER_SEARCHBASE_H_ */
