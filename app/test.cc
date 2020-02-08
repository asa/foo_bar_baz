#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_SUPER_FAST_ASSERTS
#include "external/doctest/file/doctest.h"

#include "app/core.hh"

TEST_CASE("run the app") {
    auto app = core::app();

    app.run();
}
