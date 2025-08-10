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
    void set(State next) { s = next; }
    
};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_STATUS_HPP__