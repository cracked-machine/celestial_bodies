#ifndef __COMPONENTS_COLOR_HPP__
#define __COMPONENTS_COLOR_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <components/base.hpp>
#include <random>

namespace CelestialBodies::Components {

class Color : public Base{
public:

    Color() :
        color_dist(1, 7),
        alpha_dist(0, 255),
        flicker_dist(0, 10000)
    {
        switch (color_dist(gen))
        {
            case 1 : m_value = sf::Color::White; break;
            case 2 : m_value = sf::Color::Red; break;
            case 3 : m_value = sf::Color::Green; break;
            case 4 : m_value = sf::Color::Blue; break;
            case 5 : m_value = sf::Color::Yellow; break;
            case 6 : m_value = sf::Color::Magenta; break;
            case 7 : m_value = sf::Color::Cyan; break;
        }
        
    }
    ~Color() { SPDLOG_INFO("~Color()"); }

    // randomly flicker, cooldown before relighting
    sf::Color value() 
    { 
        if( ( m_reignite_countdown == 0 ) && ( flicker_dist(gen) == 0 ) )
        { 
            m_value.a = 0; 
            m_reignite_countdown = 10; 
        }

        if( m_reignite_countdown == 0 ) {
            m_value.a = 255; 
        }
        else
        {
            m_reignite_countdown--;

        }

        return m_value;
    }
private:
    sf::Color m_value;
    int m_reignite_countdown = 0;

    static inline std::mt19937 gen{std::random_device{}()};
    std::uniform_int_distribution<> color_dist;
    std::uniform_int_distribution<> alpha_dist;
    std::uniform_int_distribution<> flicker_dist;

};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_COLOR_HPP__