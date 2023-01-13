/*
 * Client.h
 *
 *  Created on: 16 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_REDDIT_CLIENT_H_
#define SRC_REDDIT_CLIENT_H_

#include <httplib.h>
#include <chrono>

#include "src/Settings.h"
#include "src/Tools.h"

class Client {
	public:
		Client() {
			tokenClient.set_basic_auth(bot_id, bot_secret);
		}

		Json get(const std::string& address, const httplib::Params& params) {
			if (accessTokenExpiration < Clock::now()) {
				requestAccessToken();
			}

			httplib::Headers headers;
			headers.emplace("Content-Type", "application/json");
			headers.emplace("Authorization", "bearer " + accessToken);

			waitForRateLimit();
			httplib::Result result = apiClient.Get(address.c_str(), params, headers);
			resultChecks(result);

			return Json::parse(std::move(result->body));
		}

		Json get(const std::string& address) {
			if (accessTokenExpiration < Clock::now()) {
				requestAccessToken();
			}

			httplib::Headers headers;
			headers.emplace("Content-Type", "application/json");
			headers.emplace("Authorization", "bearer " + accessToken);

			waitForRateLimit();
			httplib::Result result = apiClient.Get(address.c_str(), headers);
			resultChecks(result);

			return Json::parse(std::move(result->body));
		}

		Json post(const std::string& address, const httplib::Params& params) {
			if (accessTokenExpiration < Clock::now()) {
				requestAccessToken();
			}

			httplib::Headers headers;
			headers.emplace("Content-Type", "application/json");
			headers.emplace("Authorization", "bearer " + accessToken);

			waitForRateLimit();
			httplib::Result result = apiClient.Post(address.c_str(), headers, params);
			resultChecks(result);

			return Json::parse(std::move(result->body));
		}

	private:
		httplib::Client apiClient { "https://oauth.reddit.com" };
		httplib::Client tokenClient { "https://www.reddit.com" };

		std::string accessToken;
		Clock::time_point accessTokenExpiration = Clock::time_point::min();
		Clock::time_point nextAccessTime = Clock::time_point::min();

		void resultChecks(const httplib::Result& result) {
			check(result, "Error state: ", httplib::to_string(result.error()), "\n", STD_HERE);
			check(result->status == 200, result->reason);
			check_rateLimit(result);
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
			accessTokenExpiration = std::chrono::seconds(resultJson["expires_in"].get<size_t>()) + Clock::now() - std::chrono::seconds(10);
		}

		void check_rateLimit(const httplib::Result& result) {
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
};

#endif /* SRC_REDDIT_CLIENT_H_ */
