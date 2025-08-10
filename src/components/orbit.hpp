#ifndef __COMPONENTS_ORBIT_HPP__
#define __COMPONENTS_ORBIT_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <components/base.hpp>
#include <cstddef>
#include <map>
#include <random>

namespace CelestialBodies::Components {

class Orbit : Base {
public:
    Orbit( const sf::Vector2u &position,  int requested_radius = 0, int requested_start_point = 0 ) 
    :   
        radius_dist(MIN_ORBIT_RADIUS, MAX_ORBIT_RADIUS),
        start_point_dist(0, MAX_ORBIT_RADIUS * ORBIT_RESOLUTION_MODIFIER)
    {
        float radius = 0;
        if( requested_radius ) { radius = requested_radius; }
        else { radius =  radius_dist(gen); }
        
        // pick a random starting point in the orbit trajectory
        if( requested_start_point ) { m_point = requested_start_point; }
        else { m_point = start_point_dist(gen); }
        
        m_orbit.setRadius(radius);
        m_orbit.setPointCount(radius * ORBIT_RESOLUTION_MODIFIER);
        
        // set the position of the orbit so that it is rendered in the middle of the display...ish
        m_orbit.setPosition( 
            { 
                static_cast<float>(position.x * 0.5f - radius), 
                static_cast<float>(position.y * 0.45f - radius) 
            } 
        );
        m_orbit.setFillColor( sf::Color::Transparent );
        m_orbit.setOutlineColor( sf::Color(255, 255, 255, 16) );
        m_orbit.setOutlineThickness( 1 );
    }
    ~Orbit() { SPDLOG_TRACE("~Orbit()"); }

    Orbit& operator++(int) { m_point++; return *this; }
    Orbit& operator--(int) { m_point--; return *this; }
    Orbit& operator=(const Orbit& other) { m_point = other.m_point; return *this; }
    const std::size_t get_point() { return m_point; }
    // Get the render x,y for orbital trajectory
    //  = center point + current orbital position
    const sf::Vector2f pos() const { 
        return m_orbit.getPosition() + m_orbit.getPoint(m_point); 
    }

    // Accessor: radius of orbit
    float get_radius() { return m_orbit.getRadius(); }

    // Accessor: center point of orbit
    sf::Vector2f get_center() { return m_orbit.getPosition(); }

    // Accessor: radius distribution bins
    static std::map<float, int> radius_bins() { return m_radius_bins; }

    sf::CircleShape& orbit() { return m_orbit; }

    static const int ORBIT_RESOLUTION_MODIFIER = 3;
    static const int MIN_ORBIT_RADIUS = 50;
    static const int MAX_ORBIT_RADIUS = 500;
private:
    
    // the orbit trajectory stored as points along circles circumference
    sf::CircleShape m_orbit{};
    
    // the current index in the orbit trajectory
    std::size_t m_point{};

    // rng implementation
    static inline std::mt19937 gen{std::random_device{}()};

    // rng distribution
    std::uniform_int_distribution<> radius_dist;
    std::uniform_int_distribution<> start_point_dist;

    // contains the bins of all the rng radius values used
    static std::map<float, int> m_radius_bins;

};

std::map<float, int> Orbit::m_radius_bins{};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_ORBIT_HPP__