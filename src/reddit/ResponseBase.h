/*
 * ResponseBase.h
 *
 *  Created on: 8 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_REDDIT_RESPONSEBASE_H_
#define SRC_REDDIT_RESPONSEBASE_H_

#include <vector>
#include <string>
#include <set>

#include "src/Token.h"
#include "src/Tools.h"

class ResponseBase {
protected:
	std::vector<LinkedToken> extractLinksFromLine(const std::string& line) const { // TODO needs some love
		std::vector<LinkedToken> links;

		for (size_t textBegin = line.find('['); textBegin != std::string::npos; textBegin = line.find('[', textBegin + 1)) {
			const size_t textEnd = braceEnd(line, textBegin, '[', ']');
			if (textEnd == std::string::npos) continue;

			size_t linkBegin = textEnd + 1;
			if ((line.length() > textEnd + 2) and (line[linkBegin] == '(')) {
				const size_t linkEnd = braceEnd(line, linkBegin, '(', ')');
				if (linkEnd == -1) continue;

				textBegin++;
				linkBegin++;

				if ((textBegin > textEnd) or (linkBegin > linkEnd) or (textEnd > linkBegin)) continue;

				std::string text = line.substr(textBegin, textEnd - textBegin);
				if (Token::isToken(text)) {
					std::string link = line.substr(linkBegin, linkEnd - linkBegin);
					links.push_back(LinkedToken(Token(std::move(text)), std::move(link)));
				}
			}
		}

		return links;
	}

	std::set<Token> extractStdTokensFromLine(const std::string& line) const { // TODO needs some love
		const std::string_view tokenStartString = "std::";

		std::set<Token> tokens;

		size_t tokenBegin = line.find(tokenStartString);
		while (tokenBegin != std::string::npos) {
			size_t tokenEnd = tokenBegin + tokenStartString.length();

			while (tokenEnd < line.length()) {
				if (std::isalnum(line[tokenEnd])) {
					tokenEnd++;
				} else if (line[tokenEnd] == '_') {
					tokenEnd++;
				} else if ((line[tokenEnd] == ':') and (tokenEnd + 1 < line.length()) and (line[tokenEnd + 1] == ':')) {
					tokenEnd += 2;
				} else if (line[tokenEnd] == '<') {
					tokenEnd++;
					while ((tokenEnd < line.length()) and (line[tokenEnd] != '>')) {
						tokenEnd++;
					}
					tokenEnd++;
				} else {
					break;
				}
			}

			std::string tokenValue = line.substr(tokenBegin, tokenEnd - tokenBegin); // TODO better
			if (tokenValue.back() != ':') {
				tokens.insert(Token(std::move(tokenValue)));
			}

			tokenBegin = line.find(tokenStartString, tokenEnd);
		}

		return tokens;
	}

	std::set<Token> extractHeaderTokensFromLine(const std::string& line) const { // TODO needs some love
		const char tokenStartChar = '<';
		const char tokenEndChar = '>';

		std::set<Token> tokens;

		size_t tokenBegin = line.find(tokenStartChar);
		while (tokenBegin != std::string::npos) {
			if ((tokenBegin > 0) and canBePartOfIdentifier(line[tokenBegin - 1])) {
				tokenBegin = line.find(tokenStartChar, tokenBegin + 1);
				continue;
			}

			size_t tokenEnd = tokenBegin + 1;
			while ((tokenEnd < line.length()) and canBePartOfIdentifier(line[tokenEnd])) {
				tokenEnd++;
			}

			if (tokenEnd == line.length()) {
				break;
			}

			if (line[tokenEnd] != tokenEndChar) {
				tokenBegin = line.find(tokenStartChar, tokenEnd);
				continue;
			}

			tokens.insert(Token(line.substr(tokenBegin, tokenEnd - tokenBegin + 1)));
			tokenBegin = line.find(tokenStartChar, tokenEnd + 1);
		}

		return tokens;
	}

	std::set<Token> extractTokensFromLine(std::string line) const { // TODO needs some love
		str::replace_all(line, "\\_", "_");

		std::set<Token> tokens;

		std::set<Token> std_tokens = extractStdTokensFromLine(line);
		tokens.insert(std::make_move_iterator(std_tokens.begin()),
			std::make_move_iterator(std_tokens.end()));

		std::set<Token> header_tokens = extractHeaderTokensFromLine(line);
		tokens.insert(std::make_move_iterator(header_tokens.begin()),
			std::make_move_iterator(header_tokens.end()));

		return tokens;
	}

	static bool hasKeys(const Json& json, const std::initializer_list<const char*> keys) {
		for (const char* key : keys) {
			if (!json.contains(key)) {
				return false;
			}
		}

		return true;
	}

	template<typename T>
	std::string attribute(const char* name, const T& value) const {
		std::ostringstream stream;
		stream << "[" << name << ": " << value << "]";
		return stream.str();
	}
};

#endif /* SRC_REDDIT_RESPONSEBASE_H_ */
