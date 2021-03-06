#include "frontend.h"
#include "machine.h"
#include "machine_api.h"
#include "rest_server.h"

#include <csignal>
#include <filesystem>
#include <optional>
#include <thread>

namespace
{
std::function<void()> signal_handler;
} // namespace

struct Arguments
{
    std::optional<std::filesystem::path> ui_folder;
};

Arguments parse(int argc, char *argv[])
{
    Arguments args{};
    auto pos = 1;
    if (pos < argc)
    {
        args.ui_folder = argv[pos];
    }
    return args;
}

int main(int argc, char *argv[])
{
    auto const args = parse(argc, argv);

    auto const api = std::make_shared<MachineApi>(std::make_shared<Machine>());
    auto const rest = std::make_shared<RestServer>(api);
    auto const ui = std::make_shared<Frontend>(args.ui_folder);

    std::thread rest_thread{[&] { rest->start(); }};
    std::thread ui_thread{[&] { ui->start(); }};

    signal_handler = [&] {
        ui->shutdown();
        rest->shutdown();
    };
    std::signal(SIGINT, [](int) { signal_handler(); });
    std::signal(SIGTERM, [](int) { signal_handler(); });

    rest_thread.join();
    ui_thread.join();
}
