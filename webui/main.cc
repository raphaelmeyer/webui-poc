#include <pistache/endpoint.h>
#include <pistache/router.h>

#include <nlohmann/json.hpp>

#include <map>
#include <random>
#include <string>

using namespace Pistache;

class Machine
{
public:
    Machine()
        : _state{State::off}, _rd{}, _gen{_rd()}
    {
    }

    void get_state(const Rest::Request &request, Http::ResponseWriter response)
    {
        nlohmann::json data{};

        std::map<State, std::string> const names{
            {State::off, "off"},
            {State::idle, "idle"},
            {State::running, "running"}};

        auto const name = names.find(_state);
        if (name != names.end())
        {
            response.headers().add<Http::Header::AccessControlAllowOrigin>("*"); // CORS

            data["state"] = name->second;
            response.send(Http::Code::Ok, data.dump());
        }
        else
        {
            response.send(Http::Code::Internal_Server_Error);
        }
    }

    void start(const Rest::Request &request, Http::ResponseWriter response)
    {
        switch (_state)
        {
        case State::off:
        case State::idle:
            _state = State::running;
            response.headers().add<Http::Header::AccessControlAllowOrigin>("*"); // CORS
            response.send(Http::Code::Ok);
            break;
        default:
            response.send(Http::Code::Not_Modified);
            break;
        }
    }

    void stop(const Rest::Request &request, Http::ResponseWriter response)
    {
        if (_state == State::running)
        {
            _state = State::idle;
            response.headers().add<Http::Header::AccessControlAllowOrigin>("*"); // CORS
            response.send(Http::Code::Ok);
        }
        response.send(Http::Code::Not_Modified);
    }

    void get_temperature(const Rest::Request &request, Http::ResponseWriter response)
    {
        std::uniform_real_distribution<> dis(-1.0, 1.0);
        auto const variation = dis(_gen);
        nlohmann::json data;

        switch (_state)
        {
        case State::off:
            data["temperature"] = 21.0 + variation;
            break;

        case State::idle:
            data["temperature"] = 25.0 + variation;
            break;

        case State::running:
            data["temperature"] = 42.0 + variation;
            break;
        }
        response.headers().add<Http::Header::AccessControlAllowOrigin>("*"); // CORS
        response.send(Http::Code::Ok, data.dump());
    }

private:
    enum class State
    {
        off,
        idle,
        running
    };

    State _state;

    std::random_device _rd;
    std::mt19937 _gen;
};

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

    Machine machine{};

    Rest::Routes::Get(router, "/machine/state", Rest::Routes::bind(&Machine::get_state, &machine));
    Rest::Routes::Post(router, "/machine/start", Rest::Routes::bind(&Machine::start, &machine));
    Rest::Routes::Post(router, "/machine/stop", Rest::Routes::bind(&Machine::stop, &machine));
    Rest::Routes::Get(router, "/machine/temperature", Rest::Routes::bind(&Machine::get_temperature, &machine));

    http_endpoint.setHandler(router.handler());
    http_endpoint.serve();
}
