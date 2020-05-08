#include "machine.h"

nlohmann::json Machine::get_state() const
{
    nlohmann::json state;

    switch (_state)
    {
    case State::running:
        state["state"] = "running";
        break;
    default:
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
