#pragma once

#include <nlohmann/json.hpp>

class Machine
{
public:
    nlohmann::json get_state() const;
    nlohmann::json start();

private:
    enum class State
    {
        off,
        running
    };

    State _state{State::off};
};
