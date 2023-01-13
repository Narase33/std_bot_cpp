#pragma once

#include "../src/reddit/Comment.h"
#include "../src/reddit/Thread.h"
#include "../src/Token.h"

Json createCommentJson(std::string body) {
	Json commentAsJson;
	commentAsJson["id"] = "id_0";
	commentAsJson["author"] = "author_0";
	commentAsJson["created"] = 123;
	commentAsJson["name"] = "name_0";
	commentAsJson["permalink"] = "permalink_0";
	commentAsJson["parent_id"] = "t3_parent_id_0";
	commentAsJson["link_id"] = "t3_link_id_0";
	commentAsJson["body"] = std::move(body);
	return commentAsJson;
}

Json createThreadJson(std::string body) {
	Json threadAsJson;
	threadAsJson["id"] = "id_0";
	threadAsJson["selftext"] = std::move(body);
	threadAsJson["title"] = "title_0";
	threadAsJson["permalink"] = "permalink_0";
	return threadAsJson;
}

Token createToken(std::string content, Token::Type type) {
	Token t("std::move");
	t.content = std::move(content);
	t.type = type;
	return t;
}

LinkedToken createLinkedToken(std::string content, Token::Type type, std::string link) {
	Token t("std::move");
	t.content = std::move(content);
	t.type = type;

	return LinkedToken(std::move(t), std::move(link));
}

bool containsToken(const std::set<Token>& tokens, const Token& tokenToFind) {
	return std::find_if(tokens.begin(), tokens.end(), [&](const Token& t) {
		return (t.content == tokenToFind.content)
			and (t.type == tokenToFind.type);
	}) != tokens.end();
}

bool containsLinkedToken(const std::vector<LinkedToken>& linkedTokens, const LinkedToken& linkedTokenToFind) {
	return std::find_if(linkedTokens.begin(), linkedTokens.end(), [&](const LinkedToken& t) {
		return (t.token.content == linkedTokenToFind.token.content)
			and (t.token.type == linkedTokenToFind.token.type)
			and (t.link == linkedTokenToFind.link);
	}) != linkedTokens.end();
}