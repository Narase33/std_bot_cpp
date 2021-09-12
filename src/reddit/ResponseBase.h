/*
 * ResponseBase.h
 *
 *  Created on: 8 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_REDDIT_RESPONSEBASE_H_
#define SRC_REDDIT_RESPONSEBASE_H_

#include <vector>
#include "../../libs/String.h"
#include "../Token.h"

class ResponseBase {
	protected:
		std::set<String> extractLinksFromLine(const String& line) const { // TODO needs some love
			const String httpStartString = "http";

			std::set<String> links;

			size_t linkBegin = line.find(httpStartString);
			while (linkBegin != String::npos) {
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

		std::set<Token> extractTokensFromLine(String line) const { // TODO needs some love
			const String tokenStartString = "std::";
			line.replace_all("\\_", "_");

			std::set<Token> tokens;

			size_t tokenBegin = line.find(tokenStartString);
			while (tokenBegin != String::npos) {
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

		String replaceHtmlSymbols(String str) const {
			str.replace_all("&amp;", "&");
			str.replace_all("&quot;", "\"");
			str.replace_all("&apos;", "'");
			str.replace_all("&gt;", ">");
			str.replace_all("&lt;", "<");
			str.replace_all("&#x200B;", "\"");
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
		String attribute(const char* name, const T& value) const {
			std::ostringstream stream;
			stream << "[" << name << ": " << value << "]";
			return stream.str();
		}
};

#endif /* SRC_REDDIT_RESPONSEBASE_H_ */
