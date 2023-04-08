/*
 * Tools.h
 *
 *  Created on: 5 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_TOOLS_H_
#define SRC_TOOLS_H_

#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#define STD_HERE std::string("File: ") + __FILE__ + ", Func: " + __func__ + ", Line: " + std::to_string(__LINE__)

using namespace std::string_literals;
using Clock = std::chrono::system_clock;
using Json = nlohmann::json;

namespace str {
    template <typename Arg>
    void _concat(std::ostringstream& stream, std::string_view delimiter, Arg&& arg) {
        stream << delimiter << arg;
    }

    template <typename Arg, typename... Args>
    void _concat(std::ostringstream& stream, std::string_view delimiter, Arg&& arg, Args&&... args) {
        stream << delimiter << arg;
        _concat(stream, delimiter, std::forward<Args>(args)...);
    }

    template <typename Arg, typename... Args>
    std::string concat(std::string_view delimiter, Arg&& arg, Args&&... args) {
        static_assert(sizeof...(Args) > 0, "This function needs at least 2 parameters");

        std::ostringstream stream;
        stream << arg;
        _concat(stream, delimiter, std::forward<Args>(args)...);
        return stream.str();
    }

    inline std::vector<std::string_view> split(std::string_view str, std::string_view delimiter) {
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

    template <typename Container>
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

    inline void replace_all(std::string& str, std::string_view find, std::string_view replacement) {
        size_t pos = str.find(find, 0);
        while (pos != std::string::npos) {
            str.replace(pos, find.length(), replacement);
            pos = str.find(find, pos + replacement.length());
        }
    }

    inline bool starts_with(std::string_view str, std::string_view content) {
        if (str.length() < content.length()) {
            return false;
        }

        if (str.empty()) {
            return content.empty();
        }

        for (size_t i = 0; i < content.length(); i++) {
            if (content[i] != str[i]) {
                return false;
            }
        }
        return true;
    }

    inline bool ends_with(std::string_view str, std::string_view content) {
        if (str.length() < content.length()) {
            return false;
        }

        if (str.empty()) {
            return content.empty();
        }

        for (auto str_it = str.rbegin(), content_it = content.rbegin(); content_it != content.rend(); str_it++, content_it++) {
            if (*str_it != *content_it) {
                return false;
            }
        }
        return true;
    }
} // namespace str

template <typename T1, typename... T2>
void check(bool condition, T1&& t1, T2&&... t2) {
    if (!condition) {
        throw std::runtime_error(str::concat("", "", std::forward<T1>(t1), std::forward<T2>(t2)...));
    }
}

inline std::pair<std::string_view, size_t> snip(std::string_view source, const std::string& from, const std::string& to, size_t start = 0) {
    const size_t begin = source.find(from, start);
    if (begin != std::string::npos) {
        const size_t end = source.find(to, begin + from.length());
        if (end != std::string::npos) {
            return { source.substr(begin, end + begin), end };
        }
    }

    return { "", 0 };
}

inline std::pair<std::string_view, size_t> snip_between(std::string_view source, const std::string& from, const std::string& to, size_t start = 0) {
    const size_t begin = source.find(from, start);
    if (begin != std::string::npos) {
        const size_t end = source.find(to, begin + from.length());
        if (end != std::string::npos) {
            return { source.substr(begin + from.length(), end - (begin + from.length())), end };
        }
    }

    return { "", 0 };
}

inline std::string replaceHtmlSymbols(std::string str) {
    str::replace_all(str, "&amp;", "&");
    str::replace_all(str, "&quot;", "\"");
    str::replace_all(str, "&apos;", "'");
    str::replace_all(str, "&gt;", ">");
    str::replace_all(str, "&lt;", "<");
    str::replace_all(str, "&#x200B;", "\"");
    return str;
}

inline bool canBePartOfIdentifier(char c) { // TODO Name?
    switch (c) {
        case '_':
        case ':':
        case '<':
        case '>':
        case '(':
        case ')':
        case '[':
        case ']':
            return true;
        default:
            return std::isalnum(c);
    }
}

inline size_t braceEnd(const std::string& str, size_t index, char begin, char end) {
    index++;

    int braceCount = 1;
    while ((index < str.length()) and (braceCount > 0)) {
        if (str[index] == begin) {
            braceCount++;
        }
        if (str[index] == end) {
            braceCount--;
        }
        index++;
    }

    if (braceCount > 0) {
        return std::string::npos;
    }

    return index - 1;
};

template <typename T>
void moveInsert(std::vector<T>&& from, std::vector<T>& to) {
    to.insert(to.end(), std::make_move_iterator(from.begin()), std::make_move_iterator(from.end()));
}

template <typename T>
void moveInsert(std::set<T>&& from, std::set<T>& to) {
    to.insert(std::make_move_iterator(from.begin()), std::make_move_iterator(from.end()));
}

#endif /* SRC_TOOLS_H_ */
