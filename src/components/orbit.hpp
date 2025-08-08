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

    Orbit( float windowx, float windowy ) 
        :   radius_dist(MIN_ORBIT_RADIUS, MAX_ORBIT_RADIUS),
            start_point_dist(0, MAX_ORBIT_RADIUS * ORBIT_RESOLUTION_MODIFIER)
    {
        auto radius = find_empty_orbit_neighbourhood(5);
        
        // pick a random starting point in the orbit trajectory
        m_point = start_point_dist(gen);
        
        m_orbit.setRadius(radius);
        m_orbit.setPointCount(radius * ORBIT_RESOLUTION_MODIFIER);
        // set the center of the orbit
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
    
    // Get the render x,y for orbital trajectory
    //  = center point + current orbital position
    const sf::Vector2f pos() const { 
        return m_orbit.getPosition() + m_orbit.getPoint(m_point); 
    }

    // Accessor: radius of orbit
    float get_radius() { return m_orbit.getRadius(); }
    // Accessor: orbit resolution
    std::size_t get_resolution() { return m_orbit.getPointCount(); }
    // Accessor: center point of orbit
    sf::Vector2f get_center() { return m_orbit.getPosition(); }
    // Accessor: radius distribution bins
    static std::map<int, int>& radius_bins() { return m_orbital_radius_sample_bins; }

private:
    const int ORBIT_RESOLUTION_MODIFIER = 3;
    const int MIN_ORBIT_RADIUS = 100;
    const int MAX_ORBIT_RADIUS = 900;
    
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
    static std::map<int, int> m_orbital_radius_sample_bins;

    // get a radius that doesn't have closest neighbour within `min_neighbour_distance`
    int find_empty_orbit_neighbourhood(int min_neighbour_distance)
    {
        // rng a radius, but make sure we don;t bunch up
        int radius = 0;
        assert(min_neighbour_distance < MIN_ORBIT_RADIUS); // cant go minus index on the bin map!
        while( true ) 
        {   // hopefully we don't block forever searching here
            radius = radius_dist(gen); 
            bool orbit_has_neighbours = false;
            for(int i=0; i<min_neighbour_distance; i++)
            {
                if( m_orbital_radius_sample_bins[radius + i] > 0) { orbit_has_neighbours = true; }
                if( m_orbital_radius_sample_bins[radius - i] > 0) { orbit_has_neighbours = true; }
            }
            if ( not orbit_has_neighbours ) break;
        }
        m_orbital_radius_sample_bins[radius]++;        
        return radius;
    }
};

std::map<int, int> Orbit::m_orbital_radius_sample_bins{};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_ORBIT_HPP__