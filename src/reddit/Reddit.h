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

#include "libs/httplib.h"

#include "Comment.h"
#include "Thread.h"
#include "src/Settings.h"
#include "src/Cache.h"
#include "src/Index.h"

using std::chrono::operator ""h;
using std::chrono::operator ""min;
using std::chrono::operator ""s;

class Reddit {
	public:
		Reddit() {
			tokenClient.set_basic_auth(bot_id, bot_secret);

			for (Comment& comment : requestComments()) {
				commentCache.add(std::move(comment.id));
			}
		}

		Comment requestComment(const std::string& fullName) {
			if (accessTokenExpiration < Clock::now()) {
				requestAccessToken();
			}

			httplib::Headers headers;
			headers.emplace("Content-Type", "application/json");
			headers.emplace("Authorization", "bearer " + accessToken);

			httplib::Params params;
			params.emplace("id", fullName);

			waitForRateLimit();
			const httplib::Result result = apiClient.Get("/api/info", params, headers);
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
			check(result->status == 200, result->reason);
			check_rateLimit(result);

			const Json jsonResult = Json::parse(result->body);
			return extractComments(jsonResult["data"]["children"]).front();
		}

		std::vector<Comment> requestComments() {
			const Json resultJson = oauthCall_get("/r/cpp_questions/comments");
			return extractComments(resultJson["data"]["children"]);
		}

		std::vector<Comment> requestNewComments() {
			std::vector<Comment> allComments = requestComments();

			const auto pos = std::partition(allComments.begin(), allComments.end(), [&](const Comment& c) {
				return (c.author != "std_bot") and !commentCache.isKnown(c.id);
			});

			std::vector<Comment> filteredComments;
			std::move(allComments.begin(), pos, std::back_inserter(filteredComments));

			for (const Comment& comment : filteredComments) {
				commentCache.add(comment.id);
			}

			return filteredComments;
		}

		std::pair<Thread, std::vector<Comment>> requestThread(const std::string& id) {
			const Json resultJson = oauthCall_get("/comments/" + id);

			Thread parsedThread(resultJson[0]["data"]["children"][0]["data"]);
			std::vector<Comment> parsedComments = extractComments(resultJson[1]["data"]["children"]);

			return {std::move(parsedThread), std::move(parsedComments)};
		}

		Json comment(const std::string& parentFullName, std::string payload) {
			if (accessTokenExpiration < Clock::now()) {
				requestAccessToken();
			}

			httplib::Headers headers;
			headers.emplace("Content-Type", "application/json");
			headers.emplace("Authorization", "bearer " + accessToken);

			httplib::Params params;
			params.emplace("parent", parentFullName);
			params.emplace("text", payload);

			waitForRateLimit();
			const httplib::Result result = apiClient.Post("/api/comment", headers, params);
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
			check(result->status == 200, result->reason);
			check_rateLimit(result);

			return Json::parse(result->body);
		}

	private:
		std::string accessToken;
		Clock::time_point accessTokenExpiration = Clock::time_point::min();
		Clock::time_point nextAccessTime = Clock::time_point::min();

		Cache<std::string> commentCache { std::chrono::hours(24) };
		httplib::Client apiClient { "https://oauth.reddit.com" };
		httplib::Client tokenClient { "https://www.reddit.com" };

		void check_rateLimit(const httplib::Result& result) const {
			if (std::stoi(result->get_header_value("X-Ratelimit-Remaining")) < 10) {
				printHeader("X-Ratelimit-Used", result);
				printHeader("X-Ratelimit-Remaining", result);
				printHeader("X-Ratelimit-Reset", result);

				const std::string resetTime = result->get_header_value("X-Ratelimit-Reset");
				nextAccessTime = Clock::now() + std::chrono::seconds(std::stoi(resetTime));
			}
		}

		void waitForRateLimit() const {
			if (Clock::now() < nextAccessTime) {
				std::this_thread::sleep_until(nextAccessTime);
			}
		}

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

			std::string data = "grant_type=password";
			data += "&username="s + bot_name;
			data += "&password="s + bot_password;

			const httplib::Result result = tokenClient.Post("/api/v1/access_token", data, "application/x-www-form-urlencoded");
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
			check(result->status == 200, result->reason);

			const Json resultJson = Json::parse(result->body);

			accessToken = resultJson["access_token"].get<std::string>();
			accessTokenExpiration = std::chrono::seconds(resultJson["expires_in"].get<size_t>()) + Clock::now() - 10s;
		}

		Json oauthCall_get(const std::string& address) {
			if (accessTokenExpiration < Clock::now()) {
				requestAccessToken();
			}

			httplib::Headers headers;
			headers.emplace("Content-Type", "application/json");
			headers.emplace("Authorization", "bearer " + accessToken);

			waitForRateLimit();
			const httplib::Result result = apiClient.Get(address.c_str(), headers);
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
			check(result->status == 200, result->reason);
			check_rateLimit(result);

			return Json::parse(result->body);
		}

		Json accessApiClient(std::function<httplib::Result(httplib::Client& client)> func) {
			waitForRateLimit();
			const httplib::Result result = func(apiClient);
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
			check(result->status == 200, result->reason);
			check_rateLimit(result);

			return Json::parse(result->body);
		}

		void printHeader(const char* name, const httplib::Result& result) const {
			spdlog::warn("{}: {}", name, result->get_header_value(name));
		}

		friend int main();
};

#endif /* SRC_REDDIT_H_ */
