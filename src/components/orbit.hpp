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
    // Orbit( float windowx, float windowy, int radius, int start_point )
    // :
    //     radius_dist(MIN_ORBIT_RADIUS, MAX_ORBIT_RADIUS),
    //     start_point_dist(0, MAX_ORBIT_RADIUS * ORBIT_RESOLUTION_MODIFIER)
    // {
    //     m_point = start_point;
    //     m_orbit.setRadius(radius);
    //     m_orbit.setPointCount(radius * ORBIT_RESOLUTION_MODIFIER);   

    //     // set the center of the orbit
    //     m_orbit.setPosition( 
    //         { 
    //             static_cast<float>(windowx * 0.5f - radius), 
    //             static_cast<float>(windowy * 0.5f - radius) 
    //         } 
    //     );
    //     m_orbit.setFillColor( sf::Color::Transparent );
    //     m_orbit.setOutlineColor( sf::Color(255, 255, 255, 16) );
    //     m_orbit.setOutlineThickness( 1 );
    // }

    Orbit( float windowx, float windowy,  int requested_radius = 0, int requested_start_point = 0 ) 
    :   
        radius_dist(MIN_ORBIT_RADIUS, MAX_ORBIT_RADIUS),
        start_point_dist(0, MAX_ORBIT_RADIUS * ORBIT_RESOLUTION_MODIFIER)
    {
        // auto radius = find_empty_orbit_neighbourhood(5);
        float radius = 0;
        if( requested_radius ) { radius = requested_radius; }
        else { radius =  radius_dist(gen); }
        
        // pick a random starting point in the orbit trajectory
        if( requested_start_point ) { m_point = requested_start_point; }
        else { m_point = start_point_dist(gen); }
        
        m_orbit.setRadius(radius);
        m_orbit.setPointCount(radius * ORBIT_RESOLUTION_MODIFIER);
        
        // set the center of the orbit
        m_orbit.setPosition( 
            { 
                static_cast<float>(windowx * 0.5f - radius), 
                static_cast<float>(windowy * 0.5f - radius) 
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
    // Accessor: orbit resolution
    std::size_t get_resolution() { return m_orbit.getPointCount(); }
    // Accessor: center point of orbit
    sf::Vector2f get_center() { return m_orbit.getPosition(); }
    // Accessor: radius distribution bins
    static std::map<float, int> radius_bins() { return m_radius_bins; }
    sf::CircleShape& orbit() { return m_orbit; }

    static std::pair<int, int> get_nearest_to(int radius) 
    {
        std::pair<int, int> result{};
        for(auto it = m_radius_bins.find(radius); it != m_radius_bins.end(); it++)
        {
            if( (*it).second > 0 ) { result.second = (*it).first; break; }

        }
        for(auto it = m_radius_bins.find(radius); it != m_radius_bins.begin(); it--)        
        {
            if( (*it).second > 0 ) { result.first = (*it).first; break; }
        }
        return result;
    }
    static const int ORBIT_RESOLUTION_MODIFIER = 3;
    static const int MIN_ORBIT_RADIUS = 50;
    static const int MAX_ORBIT_RADIUS = 400;
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
                if( m_radius_bins[radius + i] > 0) { orbit_has_neighbours = true; }
                if( m_radius_bins[radius - i] > 0) { orbit_has_neighbours = true; }
            }
            if ( not orbit_has_neighbours ) break;
        }
        m_radius_bins[radius]++;        
        return radius;
    }
};

std::map<float, int> Orbit::m_radius_bins{};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_ORBIT_HPP__