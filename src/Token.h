/*
 * Token.h
 *
 *  Created on: 5 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_TOKEN_H_
#define SRC_TOKEN_H_

#include <string>

struct Token {
		Token(String _content) :
				content(std::move(_content)) {
			content.replace_all("%3A", ":");
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

		String content;
};

struct LinkedToken {
		LinkedToken(Token _token, String url) :
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

		String toLink() const {
			return "[" + token.content + "](" + link + ")";
		}

		Token token;
		String link;
};

#endif /* SRC_TOKEN_H_ */
