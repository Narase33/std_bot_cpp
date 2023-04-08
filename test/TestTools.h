#pragma once

#include <catch.hpp>

#include "../src/Token.h"
#include "../src/reddit/Comment.h"
#include "../src/reddit/Thread.h"

inline Json createCommentJson(std::string body) {
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

inline Json createThreadJson(std::string body) {
    Json threadAsJson;
    threadAsJson["id"] = "id_0";
    threadAsJson["selftext"] = std::move(body);
    threadAsJson["title"] = "title_0";
    threadAsJson["permalink"] = "permalink_0";
    return threadAsJson;
}

inline Token createToken(std::string content, Token::Type type) {
    Token t("std::move");
    t.content = std::move(content);
    t.type = type;
    return t;
}

inline LinkedToken createLinkedToken(std::string content, Token::Type type, std::string link) {
    Token t("std::move");
    t.content = std::move(content);
    t.type = type;

    return LinkedToken(std::move(t), std::move(link));
}

inline bool containsToken(const std::set<Token>& tokens, const Token& tokenToFind) {
    UNSCOPED_INFO("ToFind: " + tokenToFind.content);
    for (const Token& t : tokens) {
        UNSCOPED_INFO("In list: " + t.content);
    }

    return std::find_if(tokens.begin(), tokens.end(), [&](const Token& t) {
        return (t.content == tokenToFind.content) and (t.type == tokenToFind.type);
    }) != tokens.end();
}

inline bool containsLinkedToken(const std::vector<LinkedToken>& linkedTokens, const LinkedToken& linkedTokenToFind) {
    UNSCOPED_INFO("ToFind: " + linkedTokenToFind.toLink());
    for (const LinkedToken& lt : linkedTokens) {
        UNSCOPED_INFO("In list: " + lt.toLink());
    }

    return std::find_if(linkedTokens.begin(), linkedTokens.end(), [&](const LinkedToken& t) {
        return (t.token.content == linkedTokenToFind.token.content) and (t.token.type == linkedTokenToFind.token.type) and (t.link == linkedTokenToFind.link);
    }) != linkedTokens.end();
}