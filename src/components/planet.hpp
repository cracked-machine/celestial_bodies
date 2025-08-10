#ifndef __COMPONENTS_PLANET_HPP__
#define __COMPONENTS_PLANET_HPP__

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>
#include <randint.hpp>

namespace CelestialBodies::Components {

class Planet : public sf::CircleShape {
public:
    Planet() { setRadius( rndint.gen() ); }
    Planet(int radius) { setRadius( radius ); }

    static const int PLANET_RADIUS_MIN = 10;
    static const int PLANET_RADIUS_MAX = 50;
private:
    RandInt rndint{PLANET_RADIUS_MIN, PLANET_RADIUS_MAX};
};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_PLANET_HPP__