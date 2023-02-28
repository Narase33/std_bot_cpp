#pragma once

#include "TestTools.h"
#include "catch.hpp"

#include "src/Index.h"
#include "src/linker/Linker.h"

TEST_CASE("Simple IndexTest") {
    const std::string allFormatBlock = R"(
aaaa
{}
bbbb

> cccc
> {}
> dddd

	eeee
	{}
	ffff

`gggg`
`{}`
`hhhh`

// ===== ===== ===== ===== ===== ===== ===== =====
)";
    std::string threadContent;
    threadContent += fmt::format(allFormatBlock, "std::token_unique0_0 std::token_unique0_1", "std::token_unique1_0 std::token_unique1_1", "std::token_unique2_0 std::token_unique2_1",
        "std::token_unique3_0 std::token_unique3_1");
    threadContent += fmt::format(allFormatBlock, "std::token_overwritten4_0 std::token_overwritten4_1", "std::token_overwritten5_0 std::token_overwritten5_1",
        "std::token_overwritten6_0 std::token_overwritten6_1", "std::token_overwritten7_0 std::token_overwritten7_1");
    threadContent += fmt::format(allFormatBlock, "std::token_overwritten_withLink8_0 std::token_overwritten_withLink8_1", "std::token_overwritten_withLink9_0 std::token_overwritten_withLink9_1",
        "std::token_overwritten_withLink10_0 std::token_overwritten_withLink10_1", "std::token_overwritten_withLink11_0 std::token_overwritten_withLink11_1");
    threadContent += fmt::format(allFormatBlock, "[std::token_unique12_0](link_unique_t12_0) [std::token_unique12_1](link_unique_t12_1)",
        "[std::token_unique13_0](link_unique_t13_0) [std::token_unique13_1](link_unique_t13_1)", "[std::token_unique14_0](link_unique_t14_0) [std::token_unique14_1](link_unique_t14_1)",
        "[std::token_unique15_0](link_unique_t15_0) [std::token_unique15_1](link_unique_t15_1)");
    threadContent += fmt::format(allFormatBlock, "[std::token_overwritten16_0](link_overwritten_t16_0) [std::token_overwritten16_1](link_overwritten_t16_1)",
        "[std::token_overwritten17_0](link_overwritten_t17_0) [std::token_overwritten17_1](link_overwritten_t17_1)",
        "[std::token_overwritten18_0](link_overwritten_t18_0) [std::token_overwritten18_1](link_overwritten_t18_1)",
        "[std::token_overwritten19_0](link_overwritten_t19_0) [std::token_overwritten19_1](link_overwritten_t19_1)");
    threadContent += fmt::format(allFormatBlock, "[std::token_overwritten_withLink20_0](link_overwritten_withLink_t20_0) [std::token_overwritten_withLink20_1](link_overwritten_withLink_t20_1)",
        "[std::token_overwritten_withLink21_0](link_overwritten_withLink_t21_0) [std::token_overwritten_withLink21_1](link_overwritten_withLink_t21_1)",
        "[std::token_overwritten_withLink22_0](link_overwritten_withLink_t22_0) [std::token_overwritten_withLink22_1](link_overwritten_withLink_t22_1)",
        "[std::token_overwritten_withLink23_0](link_overwritten_withLink_t23_0) [std::token_overwritten_withLink23_1](link_overwritten_withLink_t23_1)");
    threadContent +=
        fmt::format(allFormatBlock, "<header_unique0_0> <header_unique0_1>", "<header_unique1_0> <header_unique1_1>", "<header_unique2_0> <header_unique2_1>", "<header_unique3_0> <header_unique3_1>");
    threadContent += fmt::format(allFormatBlock, "<header_overwritten4_0> <header_overwritten4_1>", "<header_overwritten5_0> <header_overwritten5_1>",
        "<header_overwritten6_0> <header_overwritten6_1>", "<header_overwritten7_0> <header_overwritten7_1>");
    threadContent += fmt::format(allFormatBlock, "<header_overwritten_withLink8_0> <header_overwritten_withLink8_1>", "<header_overwritten_withLink9_0> <header_overwritten_withLink9_1>",
        "<header_overwritten_withLink10_0> <header_overwritten_withLink10_1>", "<header_overwritten_withLink11_0> <header_overwritten_withLink11_1>");
    threadContent += fmt::format(allFormatBlock, "[<header_unique12_0>](link_unique_h12_0) [<header_unique12_1>](link_unique_h12_1)",
        "[<header_unique13_0>](link_unique_h13_0) [<header_unique13_1>](link_unique_h13_1)", "[<header_unique14_0>](link_unique_h14_0) [<header_unique14_1>](link_unique_h14_1)",
        "[<header_unique15_0>](link_unique_h15_0) [<header_unique15_1>](link_unique_h15_1)");
    threadContent += fmt::format(allFormatBlock, "[<header_overwritten16_0>](link_overwritten_h16_0) [<header_overwritten16_1>](link_overwritten_h16_1)",
        "[<header_overwritten17_0>](link_overwritten_h17_0) [<header_overwritten17_1>](link_overwritten_h17_1)",
        "[<header_overwritten18_0>](link_overwritten_h18_0) [<header_overwritten18_1>](link_overwritten_h18_1)",
        "[<header_overwritten19_0>](link_overwritten_h19_0) [<header_overwritten19_1>](link_overwritten_h19_1)");
    threadContent += fmt::format(allFormatBlock, "[<header_overwritten_withLink20_0>](link_overwritten_withLink_h20_0) [<header_overwritten_withLink20_1>](link_overwritten_withLink_h20_1)",
        "[<header_overwritten_withLink21_0>](link_overwritten_withLink_h21_0) [<header_overwritten_withLink21_1>](link_overwritten_withLink_h21_1)",
        "[<header_overwritten_withLink22_0>](link_overwritten_withLink_h22_0) [<header_overwritten_withLink22_1>](link_overwritten_withLink_h22_1)",
        "[<header_overwritten_withLink23_0>](link_overwritten_withLink_h23_0) [<header_overwritten_withLink23_1>](link_overwritten_withLink_h23_1)");

    const Thread thread = createThreadJson(threadContent);

    std::vector<Comment> comments = {
        createCommentJson("std::token_overwritten0_0 std::token_overwritten1_0"),
        createCommentJson("std::token_overwritten2_0 std::token_overwritten3_0"),
        createCommentJson("std::token_overwritten0_1 std::token_overwritten1_1"),
        createCommentJson("std::token_overwritten2_1 std::token_overwritten3_1"),

        createCommentJson("[std::token_overwritten_withLink8_0](link_overwritten_withLink_t8_0_) [std::token_overwritten_withLink9_0](link_overwritten_withLink_t9_0_)"),
        createCommentJson("[std::token_overwritten_withLink10_0](link_overwritten_withLink_t10_0_) [std::token_overwritten_withLink11_0](link_overwritten_withLink_t11_0_)"),
        createCommentJson("[std::token_overwritten_withLink8_1](link_overwritten_withLink_t8_1_) [std::token_overwritten_withLink9_1](link_overwritten_withLink_t9_1_)"),
        createCommentJson("[std::token_overwritten_withLink10_1](link_overwritten_withLink_t10_1_) [std::token_overwritten_withLink11_1](link_overwritten_withLink_t11_1_)"),

        createCommentJson("std::token_overwritten16_0 std::token_overwritten17_0"),
        createCommentJson("std::token_overwritten18_0 std::token_overwritten19_0"),
        createCommentJson("std::token_overwritten16_1 std::token_overwritten17_1"),
        createCommentJson("std::token_overwritten18_1 std::token_overwritten19_1"),

        createCommentJson("[std::token_overwritten_withLink20_0](link_overwritten_withLink_t20_0_) [std::token_overwritten_withLink21_0](link_overwritten_withLink_t21_0_)"),
        createCommentJson("[std::token_overwritten_withLink22_0](link_overwritten_withLink_t22_0_) [std::token_overwritten_withLink23_0](link_overwritten_withLink_t23_0_)"),
        createCommentJson("[std::token_overwritten_withLink20_1](link_overwritten_withLink_t20_1_) [std::token_overwritten_withLink21_1](link_overwritten_withLink_t21_1_)"),
        createCommentJson("[std::token_overwritten_withLink22_1](link_overwritten_withLink_t22_1_) [std::token_overwritten_withLink23_1](link_overwritten_withLink_t23_1_)"),

        createCommentJson("<header_overwritten4_0> <header_overwritten5_0>"),
        createCommentJson("<header_overwritten6_0> <header_overwritten7_0>"),
        createCommentJson("<header_overwritten4_1> <header_overwritten5_1>"),
        createCommentJson("<header_overwritten6_1> <header_overwritten7_1>"),

        createCommentJson("[<header_overwritten_withLink8_0>](link_overwritten_withLink_h8_0_) [<header_overwritten_withLink9_0>](link_overwritten_withLink_h9_0_)"),
        createCommentJson("[<header_overwritten_withLink10_0>](link_overwritten_withLink_h10_0_) [<header_overwritten_withLink11_0>](link_overwritten_withLink_h11_0_)"),
        createCommentJson("[<header_overwritten_withLink8_1>](link_overwritten_withLink_h8_1_) [<header_overwritten_withLink9_1>](link_overwritten_withLink_h9_1_)"),
        createCommentJson("[<header_overwritten_withLink10_1>](link_overwritten_withLink_h10_1_) [<header_overwritten_withLink11_1>](link_overwritten_withLink_h11_1_)"),

        createCommentJson("<header_overwritten16_0> <header_overwritten17_0>"),
        createCommentJson("<header_overwritten18_0> <header_overwritten19_0>"),
        createCommentJson("<header_overwritten16_1> <header_overwritten17_1>"),
        createCommentJson("<header_overwritten18_1> <header_overwritten19_1>"),

        createCommentJson("[<header_overwritten_withLink20_0>](link_overwritten_withLink_h20_0_) [<header_overwritten_withLink21_0>](link_overwritten_withLink_h21_0_)"),
        createCommentJson("[<header_overwritten_withLink22_0>](link_overwritten_withLink_h22_0_) [<header_overwritten_withLink23_0>](link_overwritten_withLink_h23_0_)"),
        createCommentJson("[<header_overwritten_withLink20_1>](link_overwritten_withLink_h20_1_) [<header_overwritten_withLink21_1>](link_overwritten_withLink_h21_1_)"),
        createCommentJson("[<header_overwritten_withLink22_1>](link_overwritten_withLink_h22_1_) [<header_overwritten_withLink23_1>](link_overwritten_withLink_h23_1_)"),

        createCommentJson(fmt::format(allFormatBlock, "std::token_unique24_0 std::token_unique24_1", "std::token_unique25_0 std::token_unique25_1", "std::token_unique26_0 std::token_unique26_1",
            "std::token_unique27_0 std::token_unique27_1")),

        createCommentJson(
            fmt::format(allFormatBlock, "[std::token_unique28_0](link_t28_0_) [std::token_unique28_1](link_t28_1_)", "[std::token_unique29_0](link_t29_0_) [std::token_unique29_1](link_t29_1_)",
                "[std::token_unique30_0](link_t30_0_) [std::token_unique30_1](link_t30_1_)", "[std::token_unique31_0](link_t31_0_) [std::token_unique31_1](link_t31_1_)")),

        createCommentJson(fmt::format(allFormatBlock, "<header_unique24_0> <header_unique24_1>", "<header_unique25_0> <header_unique25_1>", "<header_unique26_0> <header_unique26_1>",
            "<header_unique27_0> <header_unique27_1>")),

        createCommentJson(fmt::format(allFormatBlock, "[<header_unique28_0>](link_h28_0_) [<header_unique28_1>](link_h28_1_)", "[<header_unique29_0>](link_h29_0_) [<header_unique29_1>](link_h29_1_)",
            "[<header_unique30_0>](link_h30_0_) [<header_unique30_1>](link_h30_1_)", "[<header_unique31_0>](link_h31_0_) [<header_unique31_1>](link_h31_1_)")),
    };

    Index index(thread, comments);

    // Thread

    CHECK(index.inIndex(createToken("std::token_unique0_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique0_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique1_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique1_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique2_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique2_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique3_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique3_1", Token::Type::std)));

    CHECK(index.inIndex(createToken("std::token_overwritten4_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten4_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten5_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten5_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten6_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten6_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten7_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten7_1", Token::Type::std)));

    CHECK(index.inIndex(createToken("std::token_overwritten_withLink8_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink8_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink9_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink9_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink10_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink10_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink11_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink11_1", Token::Type::std)));

    CHECK(index.inIndex(createToken("std::token_unique12_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique12_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique13_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique13_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique14_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique14_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique15_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique15_1", Token::Type::std)));

    CHECK(index.inIndex(createToken("std::token_overwritten16_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten16_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten17_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten17_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten18_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten18_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten19_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten19_1", Token::Type::std)));

    CHECK(index.inIndex(createToken("std::token_overwritten_withLink20_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink20_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink21_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink21_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink22_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink22_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink23_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_overwritten_withLink23_1", Token::Type::std)));

    CHECK(index.inIndex(createToken("<header_unique0_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique0_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique1_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique1_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique2_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique2_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique3_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique3_1>", Token::Type::header)));

    CHECK(index.inIndex(createToken("<header_overwritten4_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten4_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten5_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten5_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten6_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten6_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten7_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten7_1>", Token::Type::header)));

    CHECK(index.inIndex(createToken("<header_overwritten_withLink8_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink8_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink9_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink9_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink10_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink10_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink11_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink11_1>", Token::Type::header)));

    CHECK(index.inIndex(createToken("<header_unique12_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique12_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique13_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique13_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique14_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique14_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique15_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique15_1>", Token::Type::header)));

    CHECK(index.inIndex(createToken("<header_overwritten16_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten16_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten17_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten17_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten18_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten18_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten19_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten19_1>", Token::Type::header)));

    CHECK(index.inIndex(createToken("<header_overwritten_withLink20_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink20_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink21_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink21_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink22_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink22_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink23_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_overwritten_withLink23_1>", Token::Type::header)));

    // Comments

    CHECK(!index.inIndex(createToken("std::token_unique24_0", Token::Type::std)));
    CHECK(!index.inIndex(createToken("std::token_unique24_1", Token::Type::std)));
    CHECK(!index.inIndex(createToken("std::token_unique25_0", Token::Type::std)));
    CHECK(!index.inIndex(createToken("std::token_unique25_1", Token::Type::std)));
    CHECK(!index.inIndex(createToken("std::token_unique26_0", Token::Type::std)));
    CHECK(!index.inIndex(createToken("std::token_unique26_1", Token::Type::std)));
    CHECK(!index.inIndex(createToken("std::token_unique27_0", Token::Type::std)));
    CHECK(!index.inIndex(createToken("std::token_unique27_1", Token::Type::std)));

    CHECK(index.inIndex(createToken("std::token_unique28_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique28_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique29_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique29_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique30_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique30_1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique31_0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token_unique31_1", Token::Type::std)));

    CHECK(!index.inIndex(createToken("<header_unique24_0>", Token::Type::header)));
    CHECK(!index.inIndex(createToken("<header_unique24_1>", Token::Type::header)));
    CHECK(!index.inIndex(createToken("<header_unique25_0>", Token::Type::header)));
    CHECK(!index.inIndex(createToken("<header_unique25_1>", Token::Type::header)));
    CHECK(!index.inIndex(createToken("<header_unique26_0>", Token::Type::header)));
    CHECK(!index.inIndex(createToken("<header_unique26_1>", Token::Type::header)));
    CHECK(!index.inIndex(createToken("<header_unique27_0>", Token::Type::header)));
    CHECK(!index.inIndex(createToken("<header_unique27_1>", Token::Type::header)));

    CHECK(index.inIndex(createToken("<header_unique28_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique28_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique29_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique29_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique30_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique30_1>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique31_0>", Token::Type::header)));
    CHECK(index.inIndex(createToken("<header_unique31_1>", Token::Type::header)));
}

TEST_CASE("Templated IndexTest") {
    const std::string threadContent = R"(
std::token0
std::token1<T0>

std::token2::token
std::token3<T0>::token
std::token4::token<T1>
std::token5<T0>::token<T1>

std::token6::token::func
std::token7<T0>::token::func
std::token8::token<T1>::func
std::token9<T0>::token<T1>::func
std::token10::token::func<T2>
std::token11<T0>::token::func<T2>
std::token12::token<T1>::func<T2>
std::token13<T0>::token<T1>::func<T2>

std::token14::token::func()
std::token15<T0>::token::func()
std::token16::token<T1>::func()
std::token17<T0>::token<T1>::func()
std::token18::token::func<T2>()
std::token19<T0>::token::func<T2>()
std::token20::token<T1>::func<T2>()
std::token21<T0>::token<T1>::func<T2>()
)";

    Index index(createThreadJson(threadContent), {});

    CHECK(index.inIndex(createToken("std::token0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token2::token", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token3::token", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token4::token", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token5::token", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token6::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token7::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token8::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token9::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token10::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token11::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token12::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token13::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token14::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token15::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token16::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token17::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token18::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token19::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token20::token::func", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::token21::token::func", Token::Type::std)));
}

TEST_CASE("StreamingOperator IndexTest") {
    const std::string threadContent = R"(
std::cout0<<i++;
std::cout1 <<i++;
std::cout2<< i++;
std::cout3 << i++;
)";

    Index index(createThreadJson(threadContent), {});

    CHECK(index.inIndex(createToken("std::cout0", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::cout1", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::cout2", Token::Type::std)));
    CHECK(index.inIndex(createToken("std::cout3", Token::Type::std)));
}