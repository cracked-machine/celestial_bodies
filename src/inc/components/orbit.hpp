#ifndef __COMPONENTS_ORBIT_HPP__
#define __COMPONENTS_ORBIT_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <components/base.hpp>
#include <random>

namespace CelestialBodies::Components {

class Orbit : Base {
public:
    // Orbit(float radius, std::size_t vertices, sf::Vector2f center) : m_orbit(radius, vertices)  {
    //     m_orbit.setPosition( center );
    // }
    Orbit( float windowx, float windowy ) : 
        gen(std::random_device{}()), 
        dist(MIN_ORBIT_RADIUS, MAX_ORBIT_RADIUS)
    {
        auto radius = dist(gen);
        m_orbit.setRadius(radius);
        m_orbit.setPointCount(radius * 2.f);
        m_orbit.setPosition( 
            { 
                static_cast<float>(windowx * 0.5f - radius), 
                static_cast<float>(windowy * 0.5f - radius) 
            } 
        );
    }
    ~Orbit() { SPDLOG_INFO("~Orbit()"); }

    Orbit& operator++(int) { m_point++; return *this; }
    Orbit& operator--(int) { m_point--; return *this; }
    
    const sf::Vector2f pos() const
    {
        // center point + current orbital position
        return m_orbit.getPosition() + m_orbit.getPoint(m_point);
    }
    
private:
    const int MIN_ORBIT_RADIUS = 40;
    const int MAX_ORBIT_RADIUS = 400;
    std::size_t m_point{};
    sf::CircleShape m_orbit{};

    std::mt19937 gen;
    std::uniform_real_distribution<> dist;
    
};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_ORBIT_HPP__