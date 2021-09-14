/*
 * Tools.h
 *
 *  Created on: 5 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_TOOLS_H_
#define SRC_TOOLS_H_

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <string>
#include <vector>

#define STD_HERE std::string("File: ") + __FILE__ + ", Func: " + __func__ + ", Line: " + std::to_string(__LINE__)

using namespace std::string_literals;
using Clock = std::chrono::system_clock;

namespace str_tools {
	template<typename Arg>
	void _concat(std::ostringstream& stream, std::string_view delimiter, Arg&& arg) {
		stream << delimiter << arg;
	}

	template<typename Arg, typename ... Args>
	void _concat(std::ostringstream& stream, std::string_view delimiter, Arg&& arg, Args&& ... args) {
		stream << delimiter << arg;
		_concat(stream, delimiter, std::forward<Args>(args)...);
	}

	template<typename Arg, typename ... Args>
	std::string concat(std::string_view delimiter, Arg&& arg, Args&& ... args) {
		static_assert(sizeof...(Args) > 0, "This function needs at least 2 parameters");

		std::ostringstream stream;
		stream << arg;
		_concat(stream, delimiter, std::forward<Args>(args)...);
		return stream.str();
	}

	std::vector<std::string_view> split(std::string_view str, std::string_view delimiter) {
		std::vector<std::string_view> tokens;

		if (str.empty()) {
			return tokens;
		}

		size_t from = 0;
		size_t to = str.find(delimiter);

		while (to != std::string_view::npos) {
			tokens.push_back(str.substr(from, to - from));
			from = to + delimiter.length();
			to = str.find(delimiter, from);
		}

		tokens.push_back(str.substr(from, str.size() - from));
		return tokens;
	}

	template<typename Container>
	std::string join(std::string_view delimiter, const Container& container) {
		if (std::begin(container) == std::end(container)) {
			return "";
		}

		auto it = std::begin(container);
		std::ostringstream stream;
		stream << *it++;
		while (it != std::end(container)) {
			stream << delimiter << *it++;
		}

		return stream.str();
	}

	void replace_all(std::string& str, std::string_view find, std::string_view replacement) {
		size_t pos = str.find(find, 0);
		while (pos != std::string::npos) {
			str.replace(pos, find.length(), replacement);
			pos = str.find(find, pos + replacement.length());
		}
	}
}

template<typename T1, typename ... T2>
void check(bool condition, T1&& t1, T2&& ... t2) {
	if (!condition) {
		throw std::runtime_error(str_tools::concat("", "", std::forward<T1>(t1), std::forward<T2>(t2)...));
	}
}

std::pair<std::string_view, size_t> snip(std::string_view source, const std::string& from, const std::string& to, size_t start = 0) {
	const size_t begin = source.find(from, start);
	if (begin != std::string::npos) {
		const size_t end = source.find(to, begin + from.length());
		if (end != std::string::npos) {
			return {source.substr(begin, end+begin), end};
		}
	}

	return {"", 0};
}

std::pair<std::string_view, size_t> snip_between(std::string_view source, const std::string& from, const std::string& to, size_t start = 0) {
	const size_t begin = source.find(from, start);
	if (begin != std::string::npos) {
		const size_t end = source.find(to, begin + from.length());
		if (end != std::string::npos) {
			return {source.substr(begin+from.length(), end - (begin+from.length())), end};
		}
	}

	return {"", 0};
}

#endif /* SRC_TOOLS_H_ */
