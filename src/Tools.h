/*
 * Tools.h
 *
 *  Created on: 5 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_TOOLS_H_
#define SRC_TOOLS_H_

#include "../libs/String.h"

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
