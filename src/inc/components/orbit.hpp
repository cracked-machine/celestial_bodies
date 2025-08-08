#ifndef __COMPONENTS_ORBIT_HPP__
#define __COMPONENTS_ORBIT_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <components/base.hpp>

namespace CelestialBodies::Components {

class Orbit : Base {
public:
    Orbit(float radius, std::size_t vertices, sf::Vector2f center) : m_orbit(radius, vertices)  {
        m_orbit.setPosition( center );
    }

    std::size_t m_point{};
    sf::Vector2f get(std::size_t idx)
    {
        return m_orbit.getPosition() + m_orbit.getPoint(idx);
    }
private:
    sf::CircleShape m_orbit{};
};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_ORBIT_HPP__