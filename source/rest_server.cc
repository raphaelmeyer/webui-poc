#include "rest_server.h"

#include "machine_api.h"

using namespace Pistache;

RestServer::RestServer(std::shared_ptr<MachineApi> api)
    : _api{api},
      _endpoint{Address{Ipv4::any(), Port}},
      _router{}
{
}

void RestServer::start()
{
    auto const options = Http::Endpoint::options().threads(Threads);

    _endpoint.init(options);

    Rest::Routes::Get(_router, "/machine/state", Rest::Routes::bind(&MachineApi::get_state, _api));
    Rest::Routes::Post(_router, "/machine/start", Rest::Routes::bind(&MachineApi::do_start, _api));
    Rest::Routes::Post(_router, "/machine/stop", Rest::Routes::bind(&MachineApi::do_stop, _api));
    Rest::Routes::Get(_router, "/machine/temperature", Rest::Routes::bind(&MachineApi::get_temperature, _api));

    _endpoint.setHandler(_router.handler());
    _endpoint.serve();
}

void RestServer::shutdown()
{
    _endpoint.shutdown();
}