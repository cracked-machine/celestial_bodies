#ifndef __COMPONENTS_STATUS_HPP__
#define __COMPONENTS_STATUS_HPP__

#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>

namespace CelestialBodies::Components {

class Status {
public:
    enum class State
    {
        ALIVE,
        DORMANT,
        EXTINCT
    };
    State s{Status::State::ALIVE};

    State operator()() { return s; } 
    std::string to_string(State _s)
    {
        switch(static_cast<int>(_s))
        {
            case 0: return "ALIVE"; break;
            case 1: return "DORMANT"; break;
            case 2: return "EXTINCT"; break;
        }
    }
    void set(State next) { s = next; }
    
};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_STATUS_HPP__