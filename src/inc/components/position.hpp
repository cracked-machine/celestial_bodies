#ifndef __COMPONENTS_POSITION_HPP__
#define __COMPONENTS_POSITION_HPP__

#include <SFML/System/Vector2.hpp>
#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>

namespace CelestialBodies::Components {

class Position : public sf::Vector2f 
{
public:
    Position(float x, float y) : sf::Vector2f(x, y) {}
    virtual ~Position() { SPDLOG_INFO("~Position()"); }
};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_POSITION_HPP__