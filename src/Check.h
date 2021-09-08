/*
 * Check.h
 *
 *  Created on: 3 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_CHECK_H_
#define SRC_CHECK_H_

#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "../libs/String.h"

#define HERE std::string("File: ") + __FILE__ + ", Func: " + __func__ + ", Line: " + std::to_string(__LINE__)

namespace fmt {
	template<> struct formatter<String> : formatter<string_view> {
			template<typename FormatContext>
			auto format(const String& str, FormatContext& ctx) {
				return formatter<string_view>::format(str.str(), ctx);
			}
	};
}

template<typename T1, typename ... T2>
void check(bool condition, T1&& t1, T2&& ... t2) {
	if (!condition) {
		throw std::runtime_error(String().concat("", std::forward<T1>(t1), std::forward<T2>(t2)...).str());
	}
}

#endif /* SRC_CHECK_H_ */
