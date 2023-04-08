/*
 * Token.h
 *
 *  Created on: 5 Sept 2021
 *      Author: smanier
 */

#pragma once

#include "Tools.h"

struct Token {
    enum class Type { std, header };

    Token(std::string _content) :
            content(std::move(_content)) {
        str::replace_all(content, "%3A", ":");

        if (str::starts_with(content, "std::")) {
            stripToken();
            type = Type::std;
        } else if (str::starts_with(content, "<")) {
            type = Type::header;
        } else {
            throw std::runtime_error("unknown token");
        }
    }

    void stripToken() {
        removeBrace('(', ')');
        removeBrace('<', '>');
        removeBrace('[', ']');
        removeBrace('{', '}');
    }

    void removeBrace(char open, char close) {
        size_t start = content.find(open);
        while (start != std::string::npos) {
            const size_t end = braceEnd(content, start, open, close);
            if (end == std::string::npos) {
                return;
            }

            content.erase(start, end - start + 1);

            start = content.find(open);
        }
    }

    bool operator==(const Token& other) const {
        return this->content == other.content;
    }

    bool operator<(const Token& other) const {
        return this->content < other.content;
    }

    friend std::ostream& operator<<(std::ostream& o, const Token& t) {
        return o << "[content: " << t.content << "]";
    }

    std::string content;
    Type type;

    static bool isToken(const std::string& str) {
        return str::starts_with(str, "std::") or (!str.empty() and (str.front() == '<') and (str.back() == '>'));
    }
};

struct LinkedToken {
    LinkedToken(Token _token, std::string url) :
            token(std::move(_token)),
            link(std::move(url)) {
    }

    bool operator==(const LinkedToken& other) const {
        return this->token == other.token;
    }

    bool operator==(const Token& other) const {
        return this->token == other;
    }

    bool operator<(const LinkedToken& other) const {
        return this->token < other.token;
    }

    friend std::ostream& operator<<(std::ostream& o, const LinkedToken& t) {
        return o << "[token: " << t.token << ", url: " << t.link << "]";
    }

    std::string toLink() const {
        return "[" + token.content + "](" + link + ")";
    }

    Token token;
    std::string link;
};