/*
 * Tools.h
 *
 *  Created on: 5 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_TOOLS_H_
#define SRC_TOOLS_H_

#include "../libs/String.h"

#define ATTRIBUTE(x, y) Attribute<x> y{#y}

template<typename T>
class Attribute {
	public:
		Attribute(const char* n) :
				name(n) {
		}

		operator const T&() const {
			return data;
		}

		Attribute& operator=(const T& t) {
			data = t;
			return *this;
		}

		const T& operator()() const {
			return data;
		}

		const char* getName() const {
			return name;
		}

		friend std::ostream& operator<<(std::ostream& o, const Attribute& a) {
			return o << "[" << a.name << ": " << a.data << "]";
		}

	private:
		T data;
		const char* name;
};

template<typename T>
void moveContent(std::vector<T>& to, std::vector<T>& from) {
	to.insert(to.end(),
			std::make_move_iterator(from.begin()),
			std::make_move_iterator(from.end()));
}

std::pair<String, size_t> snip(const String& source, const String& from, const String& to, size_t start = 0) {
	const size_t begin = source.find(from, start);
	if (begin != String::npos) {
		const size_t end = source.find(to, begin + from.length());
		if (end != String::npos) {
			return {source.substr(begin, end - begin + from.length()), end};
		}
	}

	return {"", 0};
}

std::pair<String, size_t> snip_between(const String& source, const String& from, const String& to, size_t start = 0) {
	const size_t begin = source.find(from, start);
	if (begin != String::npos) {
		const size_t end = source.find(to, begin + from.length());
		if (end != String::npos) {
			return {source.substr(begin+from.length(), end - (begin+from.length())), end};
		}
	}

	return {"", 0};
}

#endif /* SRC_TOOLS_H_ */
