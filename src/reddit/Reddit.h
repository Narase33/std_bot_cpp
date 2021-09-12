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

using std::chrono::operator ""h;
using std::chrono::operator ""min;
using std::chrono::operator ""s;

using Clock = std::chrono::system_clock;

class Reddit {
	public:
		Reddit() {
			tokenClient.set_basic_auth(bot_id, bot_secret);
		}

		Comment requestComment(const String& fullName) {
			if (accessTokenExpiration < Clock::now()) {
				requestAccessToken();
			}

			httplib::Headers headers;
			headers.emplace("Content-Type", "application/json");
			headers.emplace("Authorization", ("bearer " + accessToken).str());

			httplib::Params params;
			params.emplace("id", fullName.str());

			const httplib::Result result = apiClient.Get("/api/info", params, headers);
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", HERE);
			check(result->status == 200, result->reason);

			const Json jsonResult = Json::parse(result->body);
			return extractComments(jsonResult["data"]["children"]).front();
		}

		std::vector<Comment> requestComments() {
			const Json resultJson = oauthCall_get("/r/cpp_questions/comments");
			return extractComments(resultJson["data"]["children"]);
		}

		std::vector<Comment> requestNewComments() {
			std::vector<Comment> allComments = requestComments();
			const Clock::time_point validCommentTimestamp = std::exchange(lastCommentRequest, Clock::now());

			const auto pos = std::partition(allComments.begin(), allComments.end(), [&](const Comment& c) {
				return ((validCommentTimestamp - Clock::time_point(std::chrono::seconds(c.created))) < 20s)
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
			if (accessTokenExpiration < Clock::now()) {
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

			if (std::stoi(result->get_header_value("X-Ratelimit-Remaining")) < 10) { // TODO Needs some delay
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
		Clock::time_point accessTokenExpiration = Clock::time_point::min();
		Clock::time_point lastCommentRequest = Clock::now();

		Cache<String> commentCache { std::chrono::minutes(1) };
		httplib::Client apiClient { "https://oauth.reddit.com" };
		httplib::Client tokenClient { "https://www.reddit.com" };

		std::vector<Comment> extractComments(const Json& dataArray) const {
			std::vector<Comment> comments;

			for (const Json& data : dataArray) {
				const Json unparsedComment = data["data"];
				if (!Comment::isCommentJson(unparsedComment)) {
					continue;
				}

				comments.push_back(Comment(unparsedComment));

				const Json replies = unparsedComment["replies"];
				if (replies.contains("data")) {
					std::vector<Comment> subComments = extractComments(replies["data"]["children"]);
					comments.insert(comments.end(),
							std::make_move_iterator(subComments.begin()),
							std::make_move_iterator(subComments.end()));
				}
			}
			return comments;
		}

		void requestAccessToken() {
			spdlog::info("Requesting new access token");

			String data = "grant_type=password";
			data += "&username="s + bot_name;
			data += "&password="s + bot_password;

			const httplib::Result result = tokenClient.Post("/api/v1/access_token", data.str(), "application/x-www-form-urlencoded");
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", HERE);
			check(result->status == 200, result->reason);

			const Json resultJson = Json::parse(result->body);

			accessToken = resultJson["access_token"].get<std::string>();
			accessTokenExpiration = std::chrono::seconds(resultJson["expires_in"].get<size_t>()) + Clock::now() - 10s;
		}

		Json oauthCall_get(const String& address) {
			if (accessTokenExpiration < Clock::now()) {
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

		void printHeader(const char* name, const httplib::Result& result) {
			std::cout << name << ": " << result->get_header_value(name) << std::endl;
		}

		friend int main();
};

#endif /* SRC_REDDIT_H_ */
