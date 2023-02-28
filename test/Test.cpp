#define CATCH_CONFIG_RUNNER
#define CATCH_CONFIG_CONSOLE_WIDTH 200

#include "catch.hpp"

#include "CommentTest.h"
#include "IndexerTest.h"
#include "LinkerTest.h"
#include "ThreadTest.h"

int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::off);

    Catch::Session session;
    session.configData().showSuccessfulTests = false;
    session.configData().showDurations = Catch::ShowDurations::Always;
    const int testReturn = session.run(argc, argv);

    return testReturn;
}