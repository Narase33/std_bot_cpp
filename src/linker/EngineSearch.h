/*
 * EngineSearch.h
 *
 *  Created on: 13 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_LINKER_ENGINESEARCH_H_
#define SRC_LINKER_ENGINESEARCH_H_

#include "SearchBase.h"

class EngineSearch: public SearchBase {
	public:
		EngineSearch(httplib::Client& client) :
				tokenClient(client) {
		}

		std::string operator()(const Token& token) {
			std::string tokenName = token.content;
			str_tools::replace_all(tokenName, "::", "%3A%3A");

			const std::string url = "/mwiki/index.php?title=Special%3ASearch&search=" + tokenName;
			httplib::Result result = tokenClient.Get(url.c_str());
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
			check(result->status == 200 or result->status == 302, result->reason);

			if (result->status == 302) { // direct hit!
				std::string redirectUrl = result->get_header_value("Location");
				redirectUrl = redirectUrl.substr(redirectUrl.find("/w"));

				result = tokenClient.Get(redirectUrl.c_str());
				check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
				check(result->status == 200, result->reason);
				return url;
			}

			const std::optional<std::string_view> link = findLinkToSubtoken(result->body, token.content, ">", "<");
			if (!link.has_value()) {
				return "";
			}

			return std::string(link.value());
		}

	private:
		httplib::Client& tokenClient;
};

#endif /* SRC_LINKER_ENGINESEARCH_H_ */