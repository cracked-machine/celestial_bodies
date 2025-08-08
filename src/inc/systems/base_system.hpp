#ifndef __SYSTEMS_BASE_SYSTEM_HPP__
#define __SYSTEMS_BASE_SYSTEM_HPP__

#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>

namespace CelestialBodies::Systems {

class BaseSystem {
public:
    BaseSystem() = default;
    ~BaseSystem() = default;
    virtual void update() = 0;
};

} // namespace CelestialBodies::Systems

#endif // __SYSTEMS_BASE_SYSTEM_HPP__