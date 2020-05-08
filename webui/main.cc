#include "machine.h"
#include "machine_api.h"

#include <pistache/endpoint.h>
#include <pistache/router.h>

#include <nlohmann/json.hpp>

#include <map>
#include <random>
#include <string>

using namespace Pistache;

int main()
{
    // auto const machine = create_machine();
    // auto const ui = create_ui();
    // auto const api = create_api(machine);
    //
    // install signal handler for ctrl+c
    //   ui->shutdown(); api->shutdown();
    //
    // api->serve();
    // ui->serve();

    // UI
    // create server on port ...
    // find files in "../ui/"
    // and add a static file serve for each file

    // REST API
    Http::Endpoint http_endpoint{Address{Ipv4::any(), 8011}};
    Rest::Router router;

    std::size_t const Threads = 4;

    auto const options = Http::Endpoint::options()
                             .threads(static_cast<int>(Threads));

    http_endpoint.init(options);

    auto const machine = std::make_shared<MachineApi>(std::make_shared<Machine>());

    Rest::Routes::Get(router, "/machine/state", Rest::Routes::bind(&MachineApi::get_state, machine));
    Rest::Routes::Post(router, "/machine/start", Rest::Routes::bind(&MachineApi::do_start, machine));
    Rest::Routes::Post(router, "/machine/stop", Rest::Routes::bind(&MachineApi::do_stop, machine));
    Rest::Routes::Get(router, "/machine/temperature", Rest::Routes::bind(&MachineApi::get_temperature, machine));

    http_endpoint.setHandler(router.handler());
    http_endpoint.serve();
}
