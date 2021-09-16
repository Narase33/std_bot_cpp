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

#include "Comment.h"
#include "Thread.h"
#include "Client.h"
#include "src/Cache.h"
#include "src/Index.h"

class Reddit {
	public:
		Reddit() {
			for (Comment& comment : requestComments()) {
				commentCache.add(std::move(comment.id));
			}
		}

		Comment requestComment(const std::string& fullName) {
			httplib::Params params;
			params.emplace("id", fullName);

			const Json resultJson = client.get("/api/info", params);
			return extractComments(resultJson["data"]["children"]).front();
		}

		std::vector<Comment> requestComments() {
			const Json resultJson = client.get("/r/cpp_questions/comments");
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
			const Json resultJson = client.get("/comments/" + id);

			Thread parsedThread(resultJson[0]["data"]["children"][0]["data"]);
			std::vector<Comment> parsedComments = extractComments(resultJson[1]["data"]["children"]);

			return {std::move(parsedThread), std::move(parsedComments)};
		}

		Json comment(const std::string& parentFullName, std::string payload) {
			httplib::Params params;
			params.emplace("parent", parentFullName);
			params.emplace("text", payload);

			return client.post("/api/comment", params);
		}

	private:
		Cache<std::string> commentCache { std::chrono::hours(24) };
		Client client;

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

		friend int main();
};

#endif /* SRC_REDDIT_H_ */
