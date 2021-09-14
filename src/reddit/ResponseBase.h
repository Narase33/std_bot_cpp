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
#include "src/Token.h"

#include "src/Tools.h"

class ResponseBase {
	protected:
		std::set<std::string> extractLinksFromLine(const std::string& line) const { // TODO needs some love
			const std::string httpStartString = "http";

			std::set<std::string> links;

			size_t linkBegin = line.find(httpStartString);
			while (linkBegin != std::string::npos) {
				size_t linkEnd = linkBegin + httpStartString.length();
				while ((linkEnd < line.length()) and (!std::isspace(line[linkEnd]) and (line[linkEnd] != ')'))) {
					linkEnd++;
				}

				if (line[linkEnd] == '.') {
					linkEnd--;
				}

				links.insert(line.substr(linkBegin, linkEnd - linkBegin));
				linkBegin = line.find(httpStartString, linkEnd);
			}

			return links;
		}

		std::set<Token> extractTokensFromLine(std::string line) const { // TODO needs some love
			const std::string_view tokenStartString = "std::";
			str_tools::replace_all(line, "\\_", "_");

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
					} else {
						break;
					}
				}

				tokens.insert(Token(line.substr(tokenBegin, tokenEnd - tokenBegin)));
				tokenBegin = line.find(tokenStartString, tokenEnd);
			}

			return tokens;
		}

		std::string replaceHtmlSymbols(std::string str) const {
			str_tools::replace_all(str, "&amp;", "&");
			str_tools::replace_all(str, "&quot;", "\"");
			str_tools::replace_all(str, "&apos;", "'");
			str_tools::replace_all(str, "&gt;", ">");
			str_tools::replace_all(str, "&lt;", "<");
			str_tools::replace_all(str, "&#x200B;", "\"");
			return str;
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
