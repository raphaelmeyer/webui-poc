#include "machine.h"

nlohmann::json Machine::get_state() const
{
    nlohmann::json state;

    switch (_state)
    {
    case State::running:
        state["state"] = "running";
        break;
    case State::idle:
        state["state"] = "idle";
        break;
    case State::off:
        state["state"] = "off";
        break;
    }

    return state;
}

nlohmann::json Machine::start()
{
    nlohmann::json result;
    if (_state == State::running)
    {
        result["start"] = "already running";
    }
    else
    {
        result["start"] = "ok";
        _state = State::running;
    }

    return result;
}

nlohmann::json Machine::stop()
{
    nlohmann::json result;
    if (_state == State::running)
    {
        result["stop"] = "ok";
        _state = State::idle;
    }
    else
    {
        result["stop"] = "not running";
    }

    return result;
}
