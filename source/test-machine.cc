#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include "machine.h"

namespace
{

TEST_CASE("Machine is initially off")
{
    Machine testee;
    CHECK(testee.get_state() == R"({"state": "off"})"_json);
}

TEST_CASE("When starting then the machine is running")
{
    Machine testee;
    CHECK(testee.start() == R"({"start": "ok"})"_json);
    CHECK(testee.get_state() == R"({"state": "running"})"_json);
}

} // namespace
