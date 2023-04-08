#include "TestTools.h"

TEST_CASE("Simple ThreadTokenTest Normal") {
    const Thread thread = createThreadJson("std::token0 std::token1 std::token2_0<T0>::token2_1<T1>::func<T2>() <header0> <header1>");

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::token0", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_0::token2_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("<header0>", Token::Type::header)));
        CHECK(containsToken(extractedTokens, createToken("<header1>", Token::Type::header)));
        REQUIRE(extractedTokens.size() == 5);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}

TEST_CASE("Simple ThreadTokenTest Marked") {
    const Thread thread = createThreadJson("`std::token0` `std::token1` `std::token2_0<T0>::token2_1<T1>::func<T2>()` `<header0>` `<header1>`");

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::token0", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_0::token2_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("<header0>", Token::Type::header)));
        CHECK(containsToken(extractedTokens, createToken("<header1>", Token::Type::header)));
        REQUIRE(extractedTokens.size() == 5);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}

TEST_CASE("Simple ThreadTokenTest Quoted") {
    const Thread thread = createThreadJson("> std::token0 std::token1 std::token2_0<T0>::token2_1<T1>::func<T2>() <header0> <header1>");

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        REQUIRE(containsToken(extractedTokens, createToken("std::token0", Token::Type::std)));
        REQUIRE(containsToken(extractedTokens, createToken("std::token1", Token::Type::std)));
        REQUIRE(containsToken(extractedTokens, createToken("std::token2_0::token2_1::func", Token::Type::std)));
        REQUIRE(containsToken(extractedTokens, createToken("<header0>", Token::Type::header)));
        REQUIRE(containsToken(extractedTokens, createToken("<header1>", Token::Type::header)));
        REQUIRE(extractedTokens.size() == 5);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}

TEST_CASE("Simple ThreadTokenTest CodeBlock") {
    const Thread thread = createThreadJson("    std::token0 std::token1 std::token2_0<T0>::token2_1<T1>::func<T2>() <header0> <header1>");

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        REQUIRE(containsToken(extractedTokens, createToken("std::token0", Token::Type::std)));
        REQUIRE(containsToken(extractedTokens, createToken("std::token1", Token::Type::std)));
        REQUIRE(containsToken(extractedTokens, createToken("std::token2_0::token2_1::func", Token::Type::std)));
        REQUIRE(containsToken(extractedTokens, createToken("<header0>", Token::Type::header)));
        REQUIRE(containsToken(extractedTokens, createToken("<header1>", Token::Type::header)));
        REQUIRE(extractedTokens.size() == 5);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}

TEST_CASE("Template ThreadTokenTest") {
    const Thread thread = createThreadJson("std::token0_0::token0_1::func "
                                           "std::token1_0<T0>::token1_1::func "
                                           "std::token2_0::token2_1<T1>::func "
                                           "std::token3_0<T0>::token3_1<T1>::func "
                                           "std::token4_0::token4_1::func<T2> "
                                           "std::token5_0<T0>::token5_1::func<T2> "
                                           "std::token6_0::token6_1<T1>::func<T2> "
                                           "std::token7_0<T0>::token7_1<T1>::func<T2> "
                                           "std::token8_0::token8_1::func() "
                                           "std::token9_0<T0>::token9_1::func() "
                                           "std::token10_0::token10_1<T1>::func() "
                                           "std::token11_0<T0>::token11_1<T1>::func() "
                                           "std::token12_0::token12_1::func<T2>() "
                                           "std::token13_0<T0>::token13_1::func<T2>() "
                                           "std::token14_0::token14_1<T1>::func<T2>() "
                                           "std::token15_0<T0>::token15_1<T1>::func<T2>()");

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::token0_0::token0_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1_0::token1_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_0::token2_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token3_0::token3_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token4_0::token4_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token5_0::token5_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token6_0::token6_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token7_0::token7_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token8_0::token8_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token9_0::token9_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token10_0::token10_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token11_0::token11_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token12_0::token12_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token13_0::token13_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token14_0::token14_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token15_0::token15_1::func", Token::Type::std)));
        REQUIRE(extractedTokens.size() == 16);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}

TEST_CASE("Template ThreadRecursiveTokenTest") {
    std::string content = R"(
std::token0_0<
	std::token1_0<
		std::token2_0,
		std::token2_1<T0>,
		std::token2_2::func()
	>,
	std::token1_1<T1>,
	std::token1_2::func()
>
::func<T1>()
)";

    std::remove_if(content.begin(), content.end(), [](char c) {
        return std::isspace(c);
    });

    const Thread thread(createThreadJson(content));

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::token0_0::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1_0", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1_1", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1_2::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_0", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_1", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_2::func", Token::Type::std)));
        REQUIRE(extractedTokens.size() == 7);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}

TEST_CASE("Simple ThreadLinkedTokenTest Normal") {
    const Thread thread(createThreadJson("[std::token0](link_t0) [std::token1](link_t1) [std::token2_0<T0>::token2_1<T1>::func<T2>()](link_t2) [<header0>](link_h0) [<header1>](link_h1)"));

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::token0", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_0::token2_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("<header0>", Token::Type::header)));
        CHECK(containsToken(extractedTokens, createToken("<header1>", Token::Type::header)));
        REQUIRE(extractedTokens.size() == 5);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();

        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token0", Token::Type::std, "link_t0")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token1", Token::Type::std, "link_t1")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token2_0::token2_1::func", Token::Type::std, "link_t2")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("<header0>", Token::Type::header, "link_h0")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("<header1>", Token::Type::header, "link_h1")));
        REQUIRE(extractedLinks.size() == 5);
    }
}

TEST_CASE("Simple ThreadLinkedTokenTest Marked") {
    const Thread thread(createThreadJson("`[std::token0](link_t0)` `[std::token1](link_t1)` `[std::token2_0<T0>::token2_1<T1>::func<T2>()](link_t2)` `[<header0>](link_h0)` `[<header1>](link_h1)`"));

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::token0", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_0::token2_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("<header0>", Token::Type::header)));
        CHECK(containsToken(extractedTokens, createToken("<header1>", Token::Type::header)));
        REQUIRE(extractedTokens.size() == 5);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();

        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token0", Token::Type::std, "link_t0")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token1", Token::Type::std, "link_t1")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token2_0::token2_1::func", Token::Type::std, "link_t2")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("<header0>", Token::Type::header, "link_h0")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("<header1>", Token::Type::header, "link_h1")));
        REQUIRE(extractedLinks.size() == 5);
    }
}

TEST_CASE("Simple ThreadLinkedTokenTest Quoted") {
    const Thread thread(createThreadJson("> [std::token0](link_t0) [std::token1](link_t1) [std::token2_0<T0>::token2_1<T1>::func<T2>()](link_t2) [<header0>](link_h0) [<header1>](link_h1)"));

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::token0", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_0::token2_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("<header0>", Token::Type::header)));
        CHECK(containsToken(extractedTokens, createToken("<header1>", Token::Type::header)));
        REQUIRE(extractedTokens.size() == 5);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();

        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token0", Token::Type::std, "link_t0")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token1", Token::Type::std, "link_t1")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token2_0::token2_1::func", Token::Type::std, "link_t2")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("<header0>", Token::Type::header, "link_h0")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("<header1>", Token::Type::header, "link_h1")));
        REQUIRE(extractedLinks.size() == 5);
    }
}

TEST_CASE("Simple ThreadLinkedTokenTest CodeBlock") {
    const Thread thread(createThreadJson("    [std::token0](link_t0) [std::token1](link_t1) [std::token2_0<T0>::token2_1<T1>::func<T2>()](link_t2) [<header0>](link_h0) [<header1>](link_h1)"));

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::token0", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_0::token2_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("<header0>", Token::Type::header)));
        CHECK(containsToken(extractedTokens, createToken("<header1>", Token::Type::header)));
        REQUIRE(extractedTokens.size() == 5);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();

        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token0", Token::Type::std, "link_t0")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token1", Token::Type::std, "link_t1")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token2_0::token2_1::func", Token::Type::std, "link_t2")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("<header0>", Token::Type::header, "link_h0")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("<header1>", Token::Type::header, "link_h1")));
        REQUIRE(extractedLinks.size() == 5);
    }
}

TEST_CASE("Template ThreadLinkedTokenTest") {
    const Thread thread = createThreadJson("[std::token0_0::token0_1::func](link0) "
                                           "[std::token1_0<T0>::token1_1::func](link1) "
                                           "[std::token2_0::token2_1<T1>::func](link2) "
                                           "[std::token3_0<T0>::token3_1<T1>::func](link3) "
                                           "[std::token4_0::token4_1::func<T2>](link4) "
                                           "[std::token5_0<T0>::token5_1::func<T2>](link5) "
                                           "[std::token6_0::token6_1<T1>::func<T2>](link6) "
                                           "[std::token7_0<T0>::token7_1<T1>::func<T2>](link7) "
                                           "[std::token8_0::token8_1::func()](link8) "
                                           "[std::token9_0<T0>::token9_1::func()](link9) "
                                           "[std::token10_0::token10_1<T1>::func()](link10) "
                                           "[std::token11_0<T0>::token11_1<T1>::func()](link11) "
                                           "[std::token12_0::token12_1::func<T2>()](link12) "
                                           "[std::token13_0<T0>::token13_1::func<T2>()](link13) "
                                           "[std::token14_0::token14_1<T1>::func<T2>()](link14) "
                                           "[std::token15_0<T0>::token15_1<T1>::func<T2>()](link15)");

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::token0_0::token0_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token1_0::token1_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token2_0::token2_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token3_0::token3_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token4_0::token4_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token5_0::token5_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token6_0::token6_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token7_0::token7_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token8_0::token8_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token9_0::token9_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token10_0::token10_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token11_0::token11_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token12_0::token12_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token13_0::token13_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token14_0::token14_1::func", Token::Type::std)));
        CHECK(containsToken(extractedTokens, createToken("std::token15_0::token15_1::func", Token::Type::std)));
        REQUIRE(extractedTokens.size() == 16);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();

        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token0_0::token0_1::func", Token::Type::std, "link0")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token1_0::token1_1::func", Token::Type::std, "link1")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token2_0::token2_1::func", Token::Type::std, "link2")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token3_0::token3_1::func", Token::Type::std, "link3")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token4_0::token4_1::func", Token::Type::std, "link4")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token5_0::token5_1::func", Token::Type::std, "link5")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token6_0::token6_1::func", Token::Type::std, "link6")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token7_0::token7_1::func", Token::Type::std, "link7")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token8_0::token8_1::func", Token::Type::std, "link8")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token9_0::token9_1::func", Token::Type::std, "link9")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token10_0::token10_1::func", Token::Type::std, "link10")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token11_0::token11_1::func", Token::Type::std, "link11")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token12_0::token12_1::func", Token::Type::std, "link12")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token13_0::token13_1::func", Token::Type::std, "link13")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token14_0::token14_1::func", Token::Type::std, "link14")));
        CHECK(containsLinkedToken(extractedLinks, createLinkedToken("std::token15_0::token15_1::func", Token::Type::std, "link15")));
        REQUIRE(extractedLinks.size() == 16);
    }
}

TEST_CASE("Simple ThreadLinkedTokenTrashTest 1") {
    const Thread thread(createThreadJson("[trash](link)"));

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();
        REQUIRE(extractedTokens.size() == 0);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}

TEST_CASE("Simple ThreadLinkedTokenTrashTest 2") {
    const Thread thread(createThreadJson("[std::trash(link)"));

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        REQUIRE(containsToken(extractedTokens, createToken("std::trash", Token::Type::std)));
        REQUIRE(extractedTokens.size() == 1);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}

TEST_CASE("Simple ThreadLinkedTokenTrashTest 3") {
    const Thread thread(createThreadJson("std::trash](link)"));

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        REQUIRE(containsToken(extractedTokens, createToken("std::trash", Token::Type::std)));
        REQUIRE(extractedTokens.size() == 1);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}

TEST_CASE("Simple ThreadLinkedTokenTrashTest 4") {
    const Thread thread(createThreadJson("[std::trash]link)"));

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::trash", Token::Type::std)));
        REQUIRE(extractedTokens.size() == 1);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}

TEST_CASE("Simple ThreadLinkedTokenTrashTest 5") {
    const Thread thread(createThreadJson("[std::trash](link"));

    SECTION("Tokens") {
        const std::set<Token> extractedTokens = thread.extractTokens();

        CHECK(containsToken(extractedTokens, createToken("std::trash", Token::Type::std)));
        REQUIRE(extractedTokens.size() == 1);
    }

    SECTION("Links") {
        const std::vector<LinkedToken> extractedLinks = thread.extractLinks();
        REQUIRE(extractedLinks.size() == 0);
    }
}
