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

TEST_CASE("Starting a running machine keeps running")
{
    Machine testee;
    testee.start();

    CHECK(testee.start() == R"({"start": "already running"})"_json);
    CHECK(testee.get_state() == R"({"state": "running"})"_json);
}

TEST_CASE("When stopping then a running machine is idle")
{
    Machine testee;
    testee.start();

    CHECK(testee.stop() == R"({"stop": "ok"})"_json);
    CHECK(testee.get_state() == R"({"state": "idle"})"_json);
}

TEST_CASE("Stopping an off machine is still off")
{
    Machine testee;
    CHECK(testee.stop() == R"({"stop": "not running"})"_json);
    CHECK(testee.get_state() == R"({"state": "off"})"_json);
}

TEST_CASE("Stopping an idle machine is still idle")
{
    Machine testee;
    testee.start();
    testee.stop();

    CHECK(testee.stop() == R"({"stop": "not running"})"_json);
    CHECK(testee.get_state() == R"({"state": "idle"})"_json);
}

} // namespace
