/*
 * Token.h
 *
 *  Created on: 5 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_TOKEN_H_
#define SRC_TOKEN_H_

#include <compare>

#include "Tools.h"

struct Token {
		Token(std::string _content) :
				content(std::move(_content)) {
			str_tools::replace_all(content, "%3A", ":");
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

#endif /* SRC_TOKEN_H_ */
