#include "machine.h"
#include "machine_api.h"
#include "rest_server.h"

#include <csignal>
#include <thread>

using namespace Pistache;

namespace
{
std::function<void()> signal_handler;
}

int main()
{
    // UI
    // create server on port ...
    // find files in "../ui/"
    // and add a static file serve for each file

    auto const api = std::make_shared<MachineApi>(std::make_shared<Machine>());
    auto const rest = std::make_shared<RestServer>(api);

    std::thread rest_thread{[&] { rest->start(); }};

    signal_handler = [&] { rest->shutdown(); };
    std::signal(SIGINT, [](int) { signal_handler(); });
    std::signal(SIGTERM, [](int) { signal_handler(); });

    rest_thread.join();
}
