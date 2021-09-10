/*
 * Reddit.h
 *
 *  Created on: 3 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_REDDIT_H_
#define SRC_REDDIT_H_

#include <iostream>
#include <iomanip>
#include <ctime>

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "../../libs/httplib.h"
#undef CPPHTTPLIB_OPENSSL_SUPPORT

#include "Comment.h"
#include "Thread.h"
#include "../Settings.h"
#include "../Check.h"
#include "../Cache.h"
#include "../Index.h"

class Reddit {
	public:
		Reddit() :
				accessTokenExpiration(0), lastCommentRequest(std::time(nullptr))
		{
			tokenClient.set_basic_auth(bot_id, bot_secret);
		}

		std::vector<Comment> requestComments() {
			const Json resultJson = oauthCall_get("/r/cpp_questions/comments");
			return extractComments(resultJson["data"]["children"]);
		}

		std::vector<Comment> requestNewComments() {
			std::vector<Comment> allComments = requestComments();
			const time_t validCommentTimestamp = std::exchange(lastCommentRequest, std::time(nullptr));

			const auto pos = std::partition(allComments.begin(), allComments.end(), [&](const Comment& c) {
				return ((validCommentTimestamp - c.created) < 20)
						and (c.author != "std_bot")
						and !commentCache.isKnown(c.id);
			});

			std::vector<Comment> filteredComments;
			std::move(allComments.begin(), pos, std::back_inserter(filteredComments));

			for (const Comment& comment : filteredComments) {
				commentCache.add(comment.id);
			}

			return filteredComments;
		}

		std::pair<Thread, std::vector<Comment>> requestThread(const String& id) {
			const Json resultJson = oauthCall_get("/comments/" + id);

			Thread parsedThread(resultJson[0]["data"]["children"][0]["data"]);
			std::vector<Comment> parsedComments = extractComments(resultJson[1]["data"]["children"]);

			return {std::move(parsedThread), std::move(parsedComments)};
		}

		Json comment(const String& parentFullName, String payload) {
			if (accessTokenExpiration < std::time(nullptr)) {
				requestAccessToken();
			}

			httplib::Headers headers;
			headers.emplace("Content-Type", "application/json");
			headers.emplace("Authorization", ("bearer " + accessToken).str());

			httplib::Params params;
			params.emplace("parent", parentFullName.str());
			params.emplace("text", payload.str());

			const httplib::Result result = apiClient.Post("/api/comment", headers, params);
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", HERE);
			check(result->status == 200, result->reason);

			if (std::stoi(result->get_header_value("X-Ratelimit-Remaining")) < 10) {
				std::cerr << "Warning! Ratelimit critical!" << std::endl;
				printHeader("X-Ratelimit-Used", result);
				printHeader("X-Ratelimit-Remaining", result);
				printHeader("X-Ratelimit-Reset", result);
				throw;
			}

			return Json::parse(result->body);
		}

	private:
		String accessToken;
		time_t accessTokenExpiration;

		time_t lastCommentRequest;

		Cache<String> commentCache { std::chrono::seconds(2) };
		httplib::Client apiClient { "https://oauth.reddit.com" };
		httplib::Client tokenClient { "https://www.reddit.com" };

		std::vector<Comment> extractComments(const Json& dataArray) const {
			std::vector<Comment> comments;

			for (const Json& data : dataArray) {
				const Json unparsedComment = data["data"];
				comments.push_back(Comment(unparsedComment));

				const Json replies = unparsedComment["replies"];
				if (replies.contains("data")) {
					std::vector<Comment> subComments = extractComments(replies["data"]["children"]);
					moveContent(comments, subComments);
				}
			}
			return comments;
		}

		void requestAccessToken() {
			spdlog::info("Requesting new access token");

			String data;
			addParams(data, "grant_type", "password");
			addParams(data, "username", bot_name);
			addParams(data, "password", bot_password);

			const httplib::Result result = tokenClient.Post("/api/v1/access_token", data.str(), "application/x-www-form-urlencoded");
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", HERE);
			check(result->status == 200, result->reason);

			const Json resultJson = Json::parse(result->body);

			accessToken = resultJson["access_token"].get<std::string>();
			accessTokenExpiration = resultJson["expires_in"].get<size_t>() + std::time(nullptr) - 10;
		}

		Json oauthCall_get(const String& address) {
			if (accessTokenExpiration < std::time(nullptr)) {
				requestAccessToken();
			}

			httplib::Headers headers;
			headers.emplace("Content-Type", "application/json");
			headers.emplace("Authorization", ("bearer " + accessToken).str());

			const httplib::Result result = apiClient.Get(address.c_str(), headers);
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", HERE);
			check(result->status == 200, result->reason);

			if (std::stoi(result->get_header_value("X-Ratelimit-Remaining")) < 10) {
				std::cerr << "Warning! Ratelimit critical!" << std::endl;
				printHeader("X-Ratelimit-Used", result);
				printHeader("X-Ratelimit-Remaining", result);
				printHeader("X-Ratelimit-Reset", result);
				throw;
			}

			return Json::parse(result->body);
		}

		void addParams(String& str, String name, String value) {
			if (!str.empty()) {
				str += '&';
			}

			str += name + "=" + value;
		}

		void printHeader(const char* name, const httplib::Result& result) {
			std::cout << name << ": " << result->get_header_value(name) << std::endl;
		}

		friend int main();
};

#endif /* SRC_REDDIT_H_ */
