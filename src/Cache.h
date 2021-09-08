/*
 * CommentCache.h
 *
 *  Created on: 3 Sept 2021
 *      Author: smanier
 */

#ifndef SRC_CACHE_H_
#define SRC_CACHE_H_

#include <vector>
#include <chrono>

template<typename T>
class Cache {
	public:
		struct Entry {
				T data;
				std::chrono::steady_clock::time_point expiration;
		};

		Cache(std::chrono::seconds time) :
				validityTime(time) {
		}

		T* tryGet(std::function<bool(const T&)> func) {
			const auto pos = std::find_if(entries.begin(), entries.end(), [&](const Entry& e) {
				return func(e.data);
			});

			if (pos != entries.end()) {
				return &pos->data;
			}

			return nullptr;
		}

		bool addIfUnknown(T data) {
			if (!isKnown(data)) {
				add(std::move(data));
				return true;
			}
			return false;
		}

		T* add(T data) {
			freeInvalidComments();
			entries.push_back( { std::move(data), std::chrono::steady_clock::now() + validityTime });
			return &entries.back().data;
		}

		bool isKnown(const T& data) {
			return std::find_if(entries.begin(), entries.end(), [&](const Entry& c) {
				return c.data == data;
			}) != entries.end();
		}

		void freeInvalidComments() {
			const auto pos = std::remove_if(entries.begin(), entries.end(), [](const Entry& c) {
				return c.expiration < std::chrono::steady_clock::now();
			});
			entries.erase(pos, entries.end());
		}

		T* find(std::function<bool(const T& t)> func) {
			auto item = std::find_if(entries.begin(), entries.end(), [&](const Entry& entry) {
				return func(entry.data);
			});

			if (item != entries.end()) {
				return &item->data;
			}
			return nullptr;
		}

	private:
		std::vector<Entry> entries;
		std::chrono::seconds validityTime;
};

#endif /* SRC_CACHE_H_ */
