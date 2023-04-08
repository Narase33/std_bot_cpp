#pragma once

#include "TestTools.h"

#include "src/linker/Linker.h"

void singleLinkerTest(const std::string& token, std::string_view link) {
    Linker linker;
    const LinkedToken* linkedToken = linker.getLinkedToken(Token(token));
    REQUIRE(linkedToken->link == link);
}

TEST_CASE("LinkerTest") {
    // singleLinkerTest("std::vector", "https://en.cppreference.com/w/cpp/container/vector");
    // singleLinkerTest("std::vector<int>", "https://en.cppreference.com/w/cpp/container/vector");

    // singleLinkerTest("std::vector::push_back", "https://en.cppreference.com/w/cpp/container/vector/push_back");
    // singleLinkerTest("std::vector::push_back()", "https://en.cppreference.com/w/cpp/container/vector/push_back");
    // singleLinkerTest("std::vector<int>::push_back", "https://en.cppreference.com/w/cpp/container/vector/push_back");
    // singleLinkerTest("std::vector<int>::push_back()", "https://en.cppreference.com/w/cpp/container/vector/push_back");

    // singleLinkerTest("std::numeric_limits<int>::min()", "https://en.cppreference.com/w/cpp/types/numeric_limits/min");

    // singleLinkerTest("std::chrono", "https://en.cppreference.com/w/cpp/chrono");
    // singleLinkerTest("std::chrono::seconds", "https://en.cppreference.com/w/cpp/chrono/duration");
    // singleLinkerTest("std::chrono::duration", "https://en.cppreference.com/mwiki/index.php?title=Special%3ASearch&search=std%3A%3Achrono%3A%3Aduration");

    // singleLinkerTest("<bitset>", "https://en.cppreference.com/w/cpp/header/bitset");
}