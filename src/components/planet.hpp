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


private:
    RandInt rndint{5, 50};
};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_PLANET_HPP__