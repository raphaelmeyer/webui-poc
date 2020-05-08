#pragma once

#include <nlohmann/json.hpp>

class Machine
{
public:
    nlohmann::json get_state() const;
};
