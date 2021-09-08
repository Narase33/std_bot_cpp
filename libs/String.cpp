/*
 * String.cpp
 *
 *  Created on: 14.09.2020
 *      Author: smanier
 */

#include "String.h"
#include <algorithm>
#include <regex>
#include <stdexcept>
#include <iomanip>

namespace {
	struct PartOf {
			PartOf(String char_set) :
					_char_set(std::move(char_set)) {
			}

			bool operator()(String::value_type ch) const {
				return std::any_of(_char_set.begin(), _char_set.end(), [&](String::value_type ch2) {
					return ch == ch2;
				});
			}

		private:
			String _char_set;
	};
}

String& String::trim_left() {
	this->erase(this->begin(), std::find_if(this->begin(), this->end(), [](int ch) {
		return !std::isspace(ch);
	}));
	return *this;
}

String& String::trim_left(const String& char_set) {
	const auto end = std::find_if_not(this->begin(), this->end(), PartOf(char_set));
	this->erase(this->begin(), end);
	return *this;
}

String& String::trim_right() {
	this->erase(std::find_if(this->rbegin(), this->rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), this->end());
	return *this;
}

String& String::trim_right(const String& char_set) {
	const auto begin = std::find_if_not(this->rbegin(), this->rend(), PartOf(char_set)).base();
	this->erase(begin, this->end());
	return *this;
}

String& String::trim() {
	this->trim_left();
	this->trim_right();
	return *this;
}

String& String::trim(const String& char_set) {
	this->trim_left(char_set);
	this->trim_right(char_set);
	return *this;
}

String::size_type String::count(char c) const {
	return std::count(_data.begin(), _data.end(), c);
}

String& String::replace_all(const String& find, const String& replacement) {
	size_type pos = this->find(find, 0);
	while (pos != String::npos) {
		this->replace(pos, find.length(), replacement);
		pos = this->find(find, pos + replacement.length());
	}
	return *this;
}

String& String::replace_all_recursive(const String& find, const String& replacement) {
	if (replacement.find(find) != String::npos)
		throw std::invalid_argument("Replacement string leads to endless recursion");

	size_type pos = this->find(find, 0);
	while (pos != String::npos) {
		this->replace(pos, find.length(), replacement);
		pos = this->find(find, pos);
	}
	return *this;
}

String::size_type String::replace_first(const String& find, const String& replacement, size_type start_pos) {
	if (this->empty())
		return String::npos;

	if (start_pos >= this->length())
		start_pos = 0;

	const size_type current_pos = this->find(find, start_pos);
	if (current_pos != String::npos) {
		this->replace(current_pos, find.length(), replacement);
	}
	return current_pos;
}

String::size_type String::replace_last(const String& find, const String& replacement, size_type start_pos) {
	if (this->empty())
		return String::npos;

	if (start_pos >= this->length())
		start_pos = this->length() - 1;

	const size_type current_pos = this->rfind(find, start_pos);
	if (current_pos != String::npos) {
		this->replace(current_pos, find.length(), replacement);
	}
	return current_pos;
}

std::vector<String> String::split(value_type delimiter) const {
	std::vector<String> tokens;

	if (this->empty())
		return tokens;

	std::string token;
	std::istringstream tokenStream(this->_data);

	while (std::getline(tokenStream, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

std::vector<String> String::split(const String& delimiter) const {
	std::vector<String> tokens;

	if (this->empty())
		return tokens;

	size_t from = 0;
	size_t to = this->find(delimiter);

	while (to != String::npos) {
		tokens.push_back(this->substr(from, to - from));
		from = to + delimiter.length();
		to = this->find(delimiter, from);
	}

	tokens.push_back(this->substr(from, this->size() - from));
	return tokens;
}

String String::split(value_type delimiter, size_t index) const {
	const std::vector<String> tokens = split(delimiter);

	if (index < tokens.size()) {
		return tokens[index];
	}

	return "";
}

String String::split(const String& delimiter, size_t index) const {
	const std::vector<String> tokens = split(delimiter);

	if (index < tokens.size()) {
		return tokens[index];
	}

	return "";
}

String String::to_lower() const {
	String out;
	out.reserve(this->length());
	std::transform(begin(), end(), std::back_inserter(out), [](char c) -> char {
		return std::tolower(c);
	});
	return out;
}

String String::to_upper() const {
	String out;
	out.reserve(this->length());
	std::transform(begin(), end(), std::back_inserter(out), [](char c) -> char {
		return std::toupper(c);
	});
	return out;
}

bool String::starts_with(const string_type& str) const {
	if (this->_data.length() < str.length()) {
		return false;
	}

	if (this->empty()) {
		return str.empty();
	}

	for (size_t i = 0; i < str.length(); i++) {
		if (str[i] != this->_data[i]) {
			return false;
		}
	}
	return true;
}

bool String::ends_with(const string_type& str) const {
	if (this->_data.length() < str.length()) {
		return false;
	}

	if (this->empty()) {
		return str.empty();
	}

	auto leftEnd = this->_data.rbegin();
	auto rightEnd = str.rbegin();
	for (size_t i = 0; i < str.length(); i++) {
		if (*leftEnd != *rightEnd) {
			return false;
		}
		leftEnd++;
		rightEnd++;
	}
	return true;
}

std::pair<String, String> String::divide(const String& delimiter, size_type delimiterIndex) const {
	if (delimiter.empty()) {
		return {"", *this};
	}

	if (delimiterIndex >= this->length()) {
		return {*this, ""};
	}

	size_t pos = 0;
	for (size_t i = 0; pos != String::npos; i++) {
		pos = this->find(delimiter, pos + 1);

		if ((pos != String::npos) && (i == delimiterIndex)) {
			return {this->substr(0, pos), this->substr(pos + delimiter.length())};
		}
	}

	return {*this,""};
}

String& String::pad_left(size_t count, char c) {
	_data = std::string(count, c) + _data;
	return *this;
}

String& String::pad_right(size_t count, char c) {
	_data += std::string(count, c);
	return *this;
}

String& String::pad_to_left(size_t endSize, char c) {
	if (endSize < this->length())
		return *this;

	const size_t remaining = endSize - this->length();
	return pad_left(remaining, c);
}

String& String::pad_to_right(size_t endSize, char c) {
	if (endSize < this->length())
		return *this;

	const size_t remaining = endSize - this->length();
	return pad_right(remaining, c);
}

String String::to_hexDump(size_t row_width) const {
	static const char characters[] = "0123456789ABCDEF";

	if (_data.empty()) {
		return "";
	}

	String out;
	out.reserve(_data.size() * 3);

	out += characters[_data[0] >> 4];
	out += characters[_data[0] & 0x0F];
	for (size_type i = 1; i < _data.length(); i++) {
		out += ((i % row_width) != 0 ? " " : "\n");
		out += characters[_data[i] >> 4];
		out += characters[_data[i] & 0x0F];
	}

	return out;
}

bool String::matches(const String::value_type* regex) const {
	const std::regex rx(regex, std::regex_constants::ECMAScript);
	return std::regex_match(this->_data, rx);
}

bool String::is_numeric() const {
	return std::all_of(_data.begin(), _data.end(), ::isdigit);
}

bool String::is_alpha() const {
	return std::all_of(_data.begin(), _data.end(), ::isalpha);
}

bool String::is_alphanumeric() const {
	return std::all_of(_data.begin(), _data.end(), ::isalnum);
}

long long String::to_longlong_or(long long value) const {
	long long result = std::atoll(_data.c_str());
	if ((result == 0) && !isStringOfZeros()) {
		return value;
	}
	return result;
}

std::optional<int> String::to_int() const {
	int result = std::atoi(_data.c_str());
	if ((result == 0) && !isStringOfZeros()) {
		return {};
	}
	return result;
}

int String::to_int_or(int value) const {
	int result = std::atoi(_data.c_str());
	if ((result == 0) && !isStringOfZeros()) {
		return value;
	}
	return result;
}

std::optional<unsigned int> String::to_uint() const {
	unsigned int result = std::atoi(_data.c_str());
	if ((result == 0) && !isStringOfZeros()) {
		return {};
	}
	return result;
}

unsigned int String::to_uint_or(unsigned int value) const {
	unsigned int result = std::atoi(_data.c_str());
	if ((result == 0) && !isStringOfZeros()) {
		return value;
	}
	return result;
}

std::optional<long> String::to_long() const {
	long result = std::atol(_data.c_str());
	if ((result == 0) && !isStringOfZeros()) {
		return {};
	}
	return result;
}

long String::to_long_or(long value) const {
	long result = std::atol(_data.c_str());
	if ((result == 0) && !isStringOfZeros()) {
		return value;
	}
	return result;
}

std::optional<long long> String::to_longlong() const {
	long long result = std::atoll(_data.c_str());
	if ((result == 0) && !isStringOfZeros()) {
		return {};
	}
	return result;
}

bool String::isStringOfZeros() const {
	return std::all_of(_data.begin(), _data.end(), [](char c) {
		return c == '0';
	});
}

