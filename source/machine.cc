#include "machine.h"

nlohmann::json Machine::get_state() const
{
    nlohmann::json state;
    state["state"] = "off";

    return state;
}
