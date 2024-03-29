/*
 * ResponseBase.h
 *
 *  Created on: 8 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_REDDIT_RESPONSEBASE_H_
#define SRC_REDDIT_RESPONSEBASE_H_

#include <set>
#include <string>
#include <vector>

#include "src/Token.h"
#include "src/Tools.h"

class ResponseBase {
  protected:
    std::vector<LinkedToken> extractLinksFromLine(const std::string& line) const { // TODO needs some love
        std::vector<LinkedToken> links;

        for (size_t textBegin = line.find('['); textBegin != std::string::npos; textBegin = line.find('[', textBegin + 1)) {
            const size_t textEnd = braceEnd(line, textBegin, '[', ']');
            if (textEnd == std::string::npos) {
                continue;
            }

            size_t linkBegin = textEnd + 1;
            if ((line.length() > textEnd + 2) and (line[linkBegin] == '(')) {
                const size_t linkEnd = braceEnd(line, linkBegin, '(', ')');
                if (linkEnd == -1) {
                    continue;
                }

                textBegin++;
                linkBegin++;

                if ((textBegin > textEnd) or (linkBegin > linkEnd) or (textEnd > linkBegin)) {
                    continue;
                }

                std::string text = line.substr(textBegin, textEnd - textBegin);
                const auto tokenBegin = std::find_if(text.begin(), text.end(), [](char c) {
                    return canBePartOfIdentifier(c);
                });
                if ((tokenBegin != text.begin()) and (tokenBegin != text.end())) {
                    text.erase(text.begin(), tokenBegin);
                }

                const auto tokenEnd = std::find_if(text.rbegin(), text.rend(), [](char c) {
                    return canBePartOfIdentifier(c);
                });
                if ((tokenEnd != text.rbegin()) and (tokenEnd != text.rend())) {
                    text.erase(tokenEnd.base(), text.end());
                }

                if (Token::isToken(text)) {
                    std::string link = line.substr(linkBegin, linkEnd - linkBegin);
                    links.push_back(LinkedToken(Token(std::move(text)), std::move(link)));
                }
            }
        }

        return links;
    }

    std::set<Token> extractStdTokensFromLine(const std::string& line) const {
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
                    const size_t templateEnd = braceEnd(line, tokenEnd, '<', '>');
                    if (templateEnd == std::string::npos) {
                        break;
                    }

                    const std::string templateContent = line.substr(tokenEnd + 1, templateEnd - tokenEnd);
                    std::set<Token> subSet = extractStdTokensFromLine(templateContent);
                    moveInsert(std::move(subSet), tokens);
                    tokenEnd = templateEnd + 1;
                } else {
                    break;
                }
            }

            std::string tokenValue = line.substr(tokenBegin, tokenEnd - tokenBegin);
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
            if ((tokenBegin > 0) and (std::isalnum(line[tokenBegin - 1])) or (line[tokenBegin - 1] == '_')) {
                tokenBegin = line.find(tokenStartChar, tokenBegin + 1);
                continue;
            }

            size_t tokenEnd = tokenBegin + 1;
            while ((tokenEnd < line.length()) and (std::isalnum(line[tokenEnd])) or (line[tokenEnd] == '_')) {
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
        tokens.insert(std::make_move_iterator(std_tokens.begin()), std::make_move_iterator(std_tokens.end()));

        std::set<Token> header_tokens = extractHeaderTokensFromLine(line);
        tokens.insert(std::make_move_iterator(header_tokens.begin()), std::make_move_iterator(header_tokens.end()));

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

    template <typename T>
    std::string attribute(const char* name, const T& value) const {
        std::ostringstream stream;
        stream << "[" << name << ": " << value << "]";
        return stream.str();
    }
};

#endif /* SRC_REDDIT_RESPONSEBASE_H_ */
