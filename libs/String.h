/*
 * StringX.h
 *
 *  Created on: 14.09.2020
 *      Author: smanier
 */

#ifndef LIBS_STRING_H_
#define LIBS_STRING_H_

#include <string>
#include <vector>
#include <sstream>
#include <optional>

class String;
std::ostream& operator<<(std::ostream& o, const String& str);
std::istream& operator>>(std::istream& i, String& str);

class String {
	public:
		using string_type = std::string;
		using value_type = string_type::value_type;
		using iterator = string_type::iterator;
		using const_iterator = string_type::const_iterator;
		using reverse_iterator = string_type::reverse_iterator;
		using const_reverse_iterator = string_type::const_reverse_iterator;
		using size_type = string_type::size_type;

		static const size_type npos = string_type::npos;

		enum class Direction {
			left, right
		};

		String() noexcept {
		}

		String(const value_type* str) :
				_data(str) {
		}

		String(const string_type& str) :
				_data(str) {
		}

		String(const String& str) :
				_data(str._data) {
		}

		String(string_type&& str) noexcept :
				_data(std::move(str)) {
		}

		String(String&& str) noexcept :
				_data(std::move(str._data)) {
		}

		String(std::size_t count, value_type ch) :
				_data(count, ch) {
		}

		String(const value_type* str, size_t count) :
				_data(str, count) {
		}

		template<class InputIt>
		String(InputIt first, InputIt last) :
				_data(first, last) {
		}

		String& operator=(const value_type* str) {
			this->_data.assign(str);
			return *this;
		}

		String& operator=(const string_type& str) {
			this->_data.assign(str);
			return *this;
		}

		String& operator=(const String& str) {
			if (this != &str)
				this->_data.assign(str._data);

			return *this;
		}

		String& operator=(string_type&& str) noexcept {
			this->_data.assign(std::move(str));
			return *this;
		}

		String& operator=(String&& str) noexcept {
			if (this != &str)
				this->_data.assign(std::move(str._data));

			return *this;
		}

		String& assign(const value_type* str) {
			this->_data.assign(str);
			return *this;
		}

		String& assign(const value_type* str, std::size_t count) {
			this->_data.assign(str, count);
			return *this;
		}

		String& assign(std::size_t count, value_type ch) {
			this->_data.assign(count, ch);
			return *this;
		}

		String& assign(const string_type& str) {
			this->_data = str;
			return *this;
		}

		String& assign(string_type&& str) noexcept {
			this->_data = std::move(str);
			return *this;
		}

		String& assign(const String& str) {
			if (this != &str)
				this->_data = str._data;
			return *this;
		}

		String& assign(String&& str) noexcept {
			if (this != &str)
				this->_data = std::move(str._data);
			return *this;
		}

		String& append(size_type count, value_type ch) {
			_data.append(count, ch);
			return *this;
		}

		String& append(const string_type& str, size_type pos, size_type count) {
			_data.append(str, pos, count);
			return *this;
		}

		String& append(const String& str, size_type pos, size_type count) {
			_data.append(str._data, pos, count);
			return *this;
		}

		String& append(const value_type* s, size_type count) {
			_data.append(s, count);
			return *this;
		}

		String& append(const value_type* s) {
			_data.append(s);
			return *this;
		}

		String& append(const string_type& str) {
			_data.append(str);
			return *this;
		}

		String& append(const String& str) {
			_data.append(str.str());
			return *this;
		}

		String& append(std::initializer_list<value_type> ilist) {
			_data.append(ilist);
			return *this;
		}

		value_type& operator[](size_t index) {
			return _data[index];
		}

		const value_type& operator[](size_t index) const {
			return _data[index];
		}

		value_type& at(size_t index) {
			return _data.at(index);
		}

		const value_type& at(size_t index) const {
			return _data.at(index);
		}

		value_type& front() {
			return _data.front();
		}

		const value_type& front() const {
			return _data.front();
		}

		value_type& back() {
			return _data.back();
		}

		const value_type& back() const {
			return _data.back();
		}

		const value_type* c_str() const {
			return _data.c_str();
		}

		const value_type* data() const {
			return _data.data();
		}

		value_type* data() {
			return _data.data();
		}

		iterator begin() {
			return _data.begin();
		}

		const_iterator begin() const {
			return _data.begin();
		}

		const_iterator cbegin() const {
			return _data.cbegin();
		}

		reverse_iterator rbegin() {
			return _data.rbegin();
		}

		const_reverse_iterator rbegin() const {
			return _data.rbegin();
		}

		const_reverse_iterator crbegin() const {
			return _data.crbegin();
		}

		iterator end() {
			return _data.end();
		}

		const_iterator end() const {
			return _data.end();
		}

		const_iterator cend() const {
			return _data.cend();
		}

		reverse_iterator rend() {
			return _data.rend();
		}

		const_reverse_iterator rend() const {
			return _data.rend();
		}

		const_reverse_iterator crend() const {
			return _data.crend();
		}

		bool empty() const {
			return _data.empty();
		}

		size_type size() const {
			return _data.size();
		}

		size_type length() const {
			return _data.length();
		}

		size_type max_size() const {
			return _data.max_size();
		}

		void reserve(size_type new_cap) {
			_data.reserve(new_cap);
		}

		size_type capacity() const {
			return _data.capacity();
		}

		void shrink_to_fit() {
			_data.shrink_to_fit();
		}

		void clear() {
			_data.clear();
		}

		String& insert(size_type index, size_type count, value_type ch) {
			_data.insert(index, count, ch);
			return *this;
		}

		String& insert(size_type index, const value_type* s) {
			_data.insert(index, s);
			return *this;
		}

		String& insert(size_type index, const value_type* s, size_type count) {
			_data.insert(index, s, count);
			return *this;
		}

		String& insert(size_type index, const string_type& str) {
			_data.insert(index, str);
			return *this;
		}

		String& insert(size_type index, const String& str) {
			_data.insert(index, str._data);
			return *this;
		}

		String& insert(size_type index, const string_type& str, size_type index_str, size_type count) {
			_data.insert(index, str, index_str, count);
			return *this;
		}

		String& insert(size_type index, const String& str, size_type index_str, size_type count) {
			_data.insert(index, str._data, index_str, count);
			return *this;
		}

		iterator insert(iterator pos, value_type ch) {
			return _data.insert(pos, ch);
		}

		void insert(iterator pos, size_type count, value_type ch) {
			_data.insert(pos, count, ch);
		}

		template<class InputIt>
		void insert(iterator pos, InputIt first, InputIt last) {
			_data.insert(pos, first, last);
		}

		void insert(iterator pos, std::initializer_list<value_type> ilist) {
			_data.insert(pos, ilist);
		}

		String& erase(size_type index = 0, size_type count = npos) {
			_data.erase(index, count);
			return *this;
		}

		iterator erase(iterator position) {
			return _data.erase(position);
		}

		iterator erase(iterator first, iterator last) {
			return _data.erase(first, last);
		}

		void push_back(value_type ch) {
			_data.push_back(ch);
		}

		void pop_back() {
			_data.pop_back();
		}

		String& operator+=(const string_type& str) {
			_data += str;
			return *this;
		}

		String& operator+=(const String& str) {
			_data += str._data;
			return *this;
		}

		String& operator+=(value_type ch) {
			_data += ch;
			return *this;
		}

		String& operator+=(const value_type* s) {
			_data += s;
			return *this;
		}

		String& operator+=(std::initializer_list<value_type> ilist) {
			_data += ilist;
			return *this;
		}

		bool starts_with(const string_type& str) const;

		bool starts_with(const String& str) const { // C++20
			if (this->empty())
				return str.empty();
			return starts_with(str._data);
		}

		bool starts_with(value_type c) const { // C++20
			if (this->empty())
				return false;
			return this->_data.front() == c;
		}

		bool starts_with(const value_type* str) const { // C++20
			if (this->empty())
				return string_type(str).empty();
			return starts_with(string_type(str));
		}

		bool ends_with(const string_type& str) const;

		bool ends_with(const String& str) const { // C++20
			if (this->empty())
				return str.empty();
			return ends_with(str._data);
		}

		bool ends_with(value_type c) const { // C++20
			if (this->empty())
				return false;
			return this->_data.back() == c;
		}

		bool ends_with(const value_type* str) const { // C++20
			if (this->empty())
				return string_type(str).empty();
			return ends_with(string_type(str));
		}

		void swap(string_type& other) {
			this->_data.swap(other);
		}

		void swap(String& other) {
			this->_data.swap(other._data);
		}

		void resize(size_type count) {
			this->_data.resize(count);
		}

		void resize(size_type count, value_type ch) {
			this->_data.resize(count, ch);
		}

		size_type copy(value_type* dest, size_type count, size_type pos = 0) const {
			return this->_data.copy(dest, count, pos);
		}

		String substr(size_type pos = 0, size_type count = npos) const {
			return this->_data.substr(pos, count);
		}

		String& replace(size_type pos, size_type count, const string_type& str) {
			_data.replace(pos, count, str);
			return *this;
		}

		String& replace(size_type pos, size_type count, const String& str) {
			_data.replace(pos, count, str._data);
			return *this;
		}

		String& replace(iterator first, iterator last, const string_type& str) {
			_data.replace(first, last, str);
			return *this;
		}

		String& replace(iterator first, iterator last, const String& str) {
			_data.replace(first, last, str._data);
			return *this;
		}

		String& replace(size_type pos, size_type count, const string_type& str, size_type pos2, size_type count2) {
			_data.replace(pos, count, str, pos2, count2);
			return *this;
		}

		String& replace(size_type pos, size_type count, const String& str, size_type pos2, size_type count2) {
			_data.replace(pos, count, str._data, pos2, count2);
			return *this;
		}

		template<class InputIt>
		String& replace(iterator first, iterator last, InputIt first2, InputIt last2) {
			_data.replace(first, last, first2, last2);
			return *this;
		}

		String& replace(size_type pos, size_type count, const value_type* cstr, size_type count2) {
			_data.replace(pos, count, cstr, count2);
			return *this;
		}

		String& replace(iterator first, iterator last, const value_type* cstr, size_type count2) {
			_data.replace(first, last, cstr, count2);
			return *this;
		}

		String& replace(size_type pos, size_type count, const value_type* cstr) {
			_data.replace(pos, count, cstr);
			return *this;
		}

		String& replace(iterator first, iterator last, const value_type* cstr) {
			_data.replace(first, last, cstr);
			return *this;
		}

		String& replace(size_type pos, size_type count, size_type count2, value_type ch) {
			_data.replace(pos, count, count2, ch);
			return *this;
		}

		String& replace(iterator first, iterator last, size_type count2, value_type ch) {
			_data.replace(first, last, count2, ch);
			return *this;
		}

		String& replace(iterator first, iterator last, std::initializer_list<value_type> ilist) {
			_data.replace(first, last, ilist);
			return *this;
		}

		template<class T>
		String& replace(size_type pos, size_type count, const T& t) {
			_data.replace(pos, count, t);
			return *this;
		}

		template<class T>
		String& replace(iterator first, iterator last, const T& t) {
			_data.replace(first, last, t);
			return *this;
		}

		template<class T>
		String& replace(size_type pos, size_type count, const T& t, size_type pos2, size_type count2 = npos) {
			_data.replace(pos, count, t, pos2, count2);
			return *this;
		}

		size_type find(const string_type& str, size_type pos = 0) const {
			return _data.find(str, pos);
		}

		size_type find(const String& str, size_type pos = 0) const {
			return _data.find(str._data, pos);
		}

		size_type find(const value_type* s, size_type pos, size_type count) const {
			return _data.find(s, pos, count);
		}

		size_type find(const value_type* s, size_type pos = 0) const {
			return _data.find(s, pos);
		}

		size_type find(value_type ch, size_type pos = 0) const {
			return _data.find(ch, pos);
		}

		bool contains(const string_type& str) const {
			return find(str) != String::npos;
		}

		bool contains(const String& str) const {
			return find(str) != String::npos;
		}

		bool contains(const value_type* str) const {
			return find(str) != String::npos;
		}

		bool contains(const value_type& str) const {
			return find(str) != String::npos;
		}

		size_type rfind(const string_type& str, size_type pos = npos) const {
			return _data.rfind(str, pos);
		}

		size_type rfind(const String& str, size_type pos = npos) const {
			return _data.rfind(str._data, pos);
		}

		size_type rfind(const value_type* s, size_type pos, size_type count) const {
			return _data.rfind(s, pos, count);
		}

		size_type rfind(const value_type* s, size_type pos = npos) const {
			return _data.rfind(s, pos);
		}

		size_type rfind(value_type ch, size_type pos = npos) const {
			return _data.rfind(ch, pos);
		}

		size_type find_first_of(const string_type& str, size_type pos = 0) const {
			return _data.find_first_of(str, pos);
		}

		size_type find_first_of(const String& str, size_type pos = 0) const {
			return _data.find_first_of(str._data, pos);
		}

		size_type find_first_of(const value_type* s, size_type pos, size_type count) const {
			return _data.find_first_of(s, pos, count);
		}

		size_type find_first_of(const value_type* s, size_type pos = 0) const {
			return _data.find_first_of(s, pos);
		}

		size_type find_first_of(value_type ch, size_type pos = 0) const {
			return _data.find_first_of(ch, pos);
		}

		size_type find_first_not_of(const string_type& str, size_type pos = 0) const {
			return _data.find_first_not_of(str, pos);
		}

		size_type find_first_not_of(const String& str, size_type pos = 0) const {
			return _data.find_first_not_of(str._data, pos);
		}

		size_type find_first_not_of(const value_type* s, size_type pos, size_type count) const {
			return _data.find_first_not_of(s, pos, count);
		}

		size_type find_first_not_of(const value_type* s, size_type pos = 0) const {
			return _data.find_first_not_of(s, pos);
		}

		size_type find_first_not_of(value_type ch, size_type pos = 0) const {
			return _data.find_first_not_of(ch, pos);
		}

		size_type find_last_of(const string_type& str, size_type pos = npos) const {
			return _data.find_last_of(str, pos);
		}

		size_type find_last_of(const String& str, size_type pos = npos) const {
			return _data.find_last_of(str._data, pos);
		}

		size_type find_last_of(const value_type* s, size_type pos, size_type count) const {
			return _data.find_last_of(s, pos, count);
		}

		size_type find_last_of(const value_type* s, size_type pos = npos) const {
			return _data.find_last_of(s, pos);
		}

		size_type find_last_of(value_type ch, size_type pos = npos) const {
			return _data.find_last_of(ch, pos);
		}

		size_type find_last_not_of(const string_type& str, size_type pos = npos) const {
			return _data.find_last_not_of(str, pos);
		}

		size_type find_last_not_of(const String& str, size_type pos = npos) const {
			return _data.find_last_not_of(str._data, pos);
		}

		size_type find_last_not_of(const value_type* s, size_type pos, size_type count) const {
			return _data.find_last_not_of(s, pos, count);
		}

		size_type find_last_not_of(const value_type* s, size_type pos = npos) const {
			return _data.find_last_not_of(s, pos);
		}

		size_type find_last_not_of(value_type ch, size_type pos = npos) const {
			return _data.find_last_not_of(ch, pos);
		}

		int compare(const string_type& str) const {
			return _data.compare(str);
		}

		int compare(const String& str) const {
			return _data.compare(str._data);
		}

		int compare(size_type pos1, size_type count1, const string_type& str) const {
			return _data.compare(pos1, count1, str);
		}

		int compare(size_type pos1, size_type count1, const String& str) const {
			return _data.compare(pos1, count1, str._data);
		}

		int compare(size_type pos1, size_type count1, const string_type& str, size_type pos2, size_type count2) const {
			return _data.compare(pos1, count1, str, pos2, count2);
		}

		int compare(size_type pos1, size_type count1, const String& str, size_type pos2, size_type count2) const {
			return _data.compare(pos1, count1, str._data, pos2, count2);
		}

		int compare(const value_type* s) const {
			return _data.compare(s);
		}

		int compare(size_type pos1, size_type count1, const value_type* s) const {
			return _data.compare(pos1, count1, s);
		}

		int compare(size_type pos1, size_type count1, const value_type* s, size_type count2) const {
			return _data.compare(pos1, count1, s, count2);
		}

		// ##### ##### ##### ##### ################ ##### ##### ##### #####
		// ##### ##### ##### ##### CUSTOM FUNCTIONS ##### ##### ##### #####
		// ##### ##### ##### ##### ################ ##### ##### ##### #####

		string_type& str() {
			return _data;
		}

		const string_type& str() const {
			return _data;
		}

		/**
		 * Converts this string to an integer
		 * In case of failure the result will be empty
		 */
		std::optional<int> to_int() const;

		/**
		 * Converts this string to an integer
		 * In case of failure the result will be 'value'
		 */

		int to_int_or(int value) const;

		/**
		 * Converts this string to an unsigned integer
		 * In case of failure the result will be empty
		 */

		std::optional<unsigned int> to_uint() const;

		/**
		 * Converts this string to an unsigned integer
		 * In case of failure the result will be 'value'
		 */
		unsigned int to_uint_or(unsigned int value) const;

		/**
		 * Converts this string to a long
		 * In case of failure the result will be empty
		 */
		std::optional<long> to_long() const;

		/**
		 * Converts this string to a long
		 * In case of failure the result will be 'value'
		 */
		long to_long_or(long value) const;

		/**
		 * Converts this string to a long long
		 * In case of failure the result will be empty
		 */
		std::optional<long long> to_longlong() const;

		/**
		 * Converts this string to a long
		 * In case of failure the result will be 'value'
		 */
		long long to_longlong_or(long long value) const;

		/**
		 * Creates a HexDump of the content of this string
		 */
		String to_hexDump(size_t row_width = String::npos) const;

		/**
		 * @return A copy of this string where every char is modified with std::to_lower
		 */
		String to_lower() const;

		/**
		 * @return A copy of this string where every char is modified with std::to_upper
		 */
		String to_upper() const;

		/**
		 * Useful when further (temporary) changes are desired
		 *
		 * @return A copy of *this
		 */
		String clone() const {
			return *this;
		}

		/**
		 * Trims the string on the left side, removing every whitespace character defined by std::isspace
		 *
		 * @return A reference to *this
		 */
		String& trim_left();

		/**
		 * Trims the string on the left side, removing every character defined in char_set
		 *
		 * @param char_set A set of chars, every char contained will be trimmed
		 *
		 * @return A reference to *this
		 */
		String& trim_left(const String& char_set);

		/**
		 * Trims the string on the right side, removing every whitespace character defined by std::isspace
		 *
		 * @return A reference to *this
		 */
		String& trim_right();

		/**
		 * Trims the string on the right side, removing every character defined in char_set
		 *
		 * @param char_set A set of chars, every char contained will be trimmed
		 *
		 * @return A reference to *this
		 */
		String& trim_right(const String& char_set);

		/**
		 * Trims the string on both sides, removing every whitespace character defined by std::isspace
		 *
		 * @return A reference to *this
		 */
		String& trim();

		/**
		 * Trims the string on both sides, removing every character defined in char_set
		 *
		 * @param char_set A set of chars, every char contained will be trimmed
		 *
		 * @return A reference to *this
		 */
		String& trim(const String& char_set);

		/**
		 * Counts the occurrences of the given char in this string
		 *
		 * @param c The char to count
		 *
		 * @return The number of occurrences of the char in this string
		 */
		size_type count(char c) const;

		/**
		 * Replaces all occurrences of the given string with its replacement
		 * "aaaaaa".replace_all("aa", "a) -> "aaa"
		 *
		 * @param find The string to replace
		 * @param replacement The string written in place of the replaced string
		 *
		 * @return A reference to *this
		 */
		String& replace_all(const String& find, const String& replacement);

		/**
		 * Replaces all occurrences of the given string with its replacement recursively
		 * "aaaaaa".replace_all("aa", "a) -> "a"
		 *
		 * @param find The string to replace
		 * @param replacement The string written in place of the replaced string
		 *
		 * @throws std::invalid_argument if the given replacement would lead to an endless recursion
		 *
		 * @return A reference to *this
		 */
		String& replace_all_recursive(const String& find, const String& replacement);

		/**
		 * Replaces the first occurrence of the given string with its replacement
		 *
		 * @param find The string to replace
		 * @param replacement The string written in place of the replaced string
		 * @param start_pos The position where to start searching. If the range is invalid, search starts from 0
		 *
		 * @return The pos of the occurrence or npos if nothing was found
		 */
		size_type replace_first(const String& find, const String& replacement, size_type start_pos = 0);

		/**
		 * Replaces the last occurrence of the given string with its replacement
		 *
		 * @param find The string to replace
		 * @param replacement The string written in place of the replaced string
		 * @param start_pos The position where to start searching. If the range is invalid, search starts from 'npos'
		 *
		 * @return The pos of the occurrence or npos if nothing was found
		 */
		size_type replace_last(const String& find, const String& replacement, size_type start_pos = npos);

		/**
		 * Splits this string given a specific separator
		 * For single character delimiter this function is faster than 'split(const String& delimiter)'
		 *
		 * @param delimiter The separator used to split this string. It will not be included in the result
		 *
		 * @result The list of strings. The order of the list will be the same as they were in this string
		 */
		std::vector<String> split(value_type delimiter) const;

		/**
		 * Splits this string given a specific separator
		 * For single character delimiter this function is slower than 'split(value_type delimiter)'
		 *
		 * @param delimiter The separator used to split this string. It will not be included in the result
		 *
		 * @result The list of strings. The order of the list will be the same as they were in this string
		 */
		std::vector<String> split(const String& delimiter = " ") const;

		/**
		 * Splits this string given a specific separator and returns the index' position or "" if too high
		 * For single character delimiter this function is slower than 'split(value_type delimiter)'
		 *
		 * @param delimiter The separator used to split this string. It will not be included in the result
		 *
		 * @result The index'th string from the split-array or empty-string if the index is too high
		 */
		String split(value_type delimiter, size_t index) const;

		/**
		 * Splits this string given a specific separator and returns the index' position or "" if too high
		 * For single character delimiter this function is slower than 'split(value_type delimiter)'
		 *
		 * @param delimiter The separator used to split this string. It will not be included in the result
		 *
		 * @result The index'th string from the split-array or empty-string if the index is too high
		 */
		String split(const String& delimiter, size_t index) const;

		/**
		 * Divides this string at the 'index'th occurrence of the delimiter
		 * If the delimiter is an empty string, the result is ["", *this]
		 * If the delimiter is not found, the result is [*this, ""]
		 * If the index is out of range, the result is [*this, ""]
		 *
		 * @param delimiter The separator which divides the string
		 * @param index The n'th occurrence of the delimiter at which the string is divided
		 *
		 * @return A pair of Strings, split at the required place
		 */
		std::pair<String, String> divide(const String& delimiter = " ", size_type index = 0) const;

		/**
		 * Checks if this string matches the given regex
		 * NOT YET IMPLEMENTED
		 *
		 * @param The regex to check against this string
		 *
		 * @return true if the regex matches or false if it doesnt match
		 */
		bool matches(const value_type* regex) const;

		bool matches(const std::string& regex) const {
			return matches(regex.c_str());
		}

		bool matches(const String& regex) const {
			return matches(regex.c_str());
		}

		/**
		 * Pads this string with the given char on the left side
		 *
		 * @return *this
		 */
		String& pad_left(size_t count, char c);

		/**
		 * Pads this string with the given char on the right side
		 *
		 * @return *this
		 */
		String& pad_right(size_t count, char c);

		/**
		 * Pads this string to the given size with the given char on the left side
		 * If endSize <= this->length() nothing happens
		 *
		 * @return *this
		 */
		String& pad_to_left(size_t endSize, char c);

		/**
		 * Pads this string to the given size with the given char on the right side
		 * If endSize <= this->length() nothing happens
		 *
		 * @return *this
		 */
		String& pad_to_right(size_t endSize, char c);

		bool is_numeric() const;
		bool is_alpha() const;
		bool is_alphanumeric() const;

		/**
		 * Joins the elements given in the container with this string as separator
		 * The container must fulfill the requirements of std::begin(T) and std::end(T) giving a 'LegacyInputIterator'
		 * The elements of the container need to be streamable
		 * This function will alter this string
		 * If the container is empty, this function will clear() this string and return it, thus the result will be ""
		 *
		 * @param container The container with streamable objects
		 *
		 * @return *this
		 */
		template<typename Container>
		String& join(const Container& container) {
			if (std::begin(container) == std::end(container)) {
				_data.clear();
				return *this;
			}

			auto it = std::begin(container);
			std::ostringstream stream;
			stream << *it++;
			while (it != std::end(container)) {
				stream << _data << *it++;
			}

			_data = stream.str();
			return *this;
		}

		/**
		 * Concats the elements given in the container with this string as separator
		 * The elements need to be streamable
		 * This function will alter this string
		 * Like 'String::join(Container)' but for a static number of elements with different types
		 *
		 * @param container The container with streamable objects
		 *
		 * @return *this
		 */
		template<typename Arg, typename ... Args>
		String& concat(Arg&& arg, Args&& ... args) {
			static_assert(sizeof...(Args) > 0, "This function needs at least 2 parameters");

			std::ostringstream stream;
			stream << arg;
			_concat(stream, std::forward<Args>(args)...);
			_data = stream.str();
			return *this;
		}
	private:
		string_type _data;

		template<typename Arg>
		void _concat(std::ostringstream& stream, Arg&& arg) const {
			stream << _data << arg;
		}

		template<typename Arg, typename ... Args>
		void _concat(std::ostringstream& stream, Arg&& arg, Args&& ... args) const {
			stream << _data << arg;
			_concat(stream, std::forward<Args>(args)...);
		}

		bool isStringOfZeros() const;
};

inline String operator+(const String& a, const String& b) {
	return a.str() + b.str();
}

inline String operator+(const String& a, const String::string_type& b) {
	return a.str() + b;
}

inline String operator+(const String::string_type& a, const String& b) {
	return a + b.str();
}

inline String operator+(const String& a, String::value_type ch) {
	return a.str() + ch;
}

inline String operator+(String::value_type a, const String& b) {
	return a + b.str();
}

inline String operator+(const String& a, const String::value_type* b) {
	return a.str() + b;
}

inline String operator+(const String::value_type* a, const String& b) {
	return a + b.str();
}

inline std::ostream& operator<<(std::ostream& o, const String& str) {
	return o << str.str();
}

inline std::istream& operator>>(std::istream& i, String& str) {
	return i >> str.str();
}

inline bool operator==(const String& a, const String& b) {
	return a.compare(b) == 0;
}

inline bool operator==(const String& a, const String::string_type& b) {
	return a.compare(b) == 0;
}

inline bool operator==(const String::string_type& a, const String& b) {
	return a.compare(b.str()) == 0;
}

inline bool operator==(const String& a, const String::value_type* b) {
	return a.compare(b) == 0;
}

inline bool operator==(const String::value_type* a, const String& b) {
	return String(a).compare(b) == 0;
}

inline bool operator!=(const String& a, const String& b) {
	return a.compare(b) != 0;
}

inline bool operator!=(const String& a, const String::string_type& b) {
	return a.compare(b) != 0;
}

inline bool operator!=(const String::string_type& a, const String& b) {
	return a.compare(b.str()) != 0;
}

inline bool operator!=(const String& a, const String::value_type* b) {
	return a.compare(b) != 0;
}

inline bool operator!=(const String::value_type* a, const String& b) {
	return String(a).compare(b) != 0;
}

inline bool operator<(const String& a, const String& b) {
	return a.compare(b) < 0;
}

inline bool operator<(const String& a, const String::string_type& b) {
	return a.compare(b) < 0;
}

inline bool operator<(const String::string_type& a, const String& b) {
	return a.compare(b.str()) < 0;
}

inline bool operator<(const String& a, const String::value_type* b) {
	return a.compare(b) < 0;
}

inline bool operator<(const String::value_type* a, const String& b) {
	return String(a).compare(b) < 0;
}

inline bool operator<=(const String& a, const String& b) {
	return a.compare(b) <= 0;
}

inline bool operator<=(const String& a, const String::string_type& b) {
	return a.compare(b) <= 0;
}

inline bool operator<=(const String::string_type& a, const String& b) {
	return a.compare(b.str()) <= 0;
}

inline bool operator<=(const String& a, const String::value_type* b) {
	return a.compare(b) <= 0;
}

inline bool operator<=(const String::value_type* a, const String& b) {
	return String(a).compare(b) <= 0;
}

inline bool operator>(const String& a, const String& b) {
	return a.compare(b) > 0;
}

inline bool operator>(const String& a, const String::string_type& b) {
	return a.compare(b) > 0;
}

inline bool operator>(const String::string_type& a, const String& b) {
	return a.compare(b.str()) > 0;
}

inline bool operator>(const String& a, const String::value_type* b) {
	return a.compare(b) > 0;
}

inline bool operator>(const String::value_type* a, const String& b) {
	return String(a).compare(b) > 0;
}

inline bool operator>=(const String& a, const String& b) {
	return a.compare(b) >= 0;
}

inline bool operator>=(const String& a, const String::string_type& b) {
	return a.compare(b) >= 0;
}

inline bool operator>=(const String::string_type& a, const String& b) {
	return a.compare(b.str()) >= 0;
}

inline bool operator>=(const String& a, const String::value_type* b) {
	return a.compare(b) >= 0;
}

inline bool operator>=(const String::value_type* a, const String& b) {
	return String(a).compare(b) >= 0;
}

namespace std {
	template<>
	struct hash<String> {
			size_t operator()(const String& str) const {
				return hash()(str.str());
			}
	};
}

#endif /* VERALIB_STRING_H_ */
