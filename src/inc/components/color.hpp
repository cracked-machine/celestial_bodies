#ifndef __COMPONENTS_COLOR_HPP__
#define __COMPONENTS_COLOR_HPP__

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <components/base.hpp>

namespace CelestialBodies::Components {

class Color : public Base{
public:

    Color(int c, std::uint8_t alpha = 255)   {
        switch (c)
        {
            case 1 : m_value = sf::Color::White; break;
            case 2 : m_value = sf::Color::Red; break;
            case 3 : m_value = sf::Color::Green; break;
            case 4 : m_value = sf::Color::Blue; break;
            case 5 : m_value = sf::Color::Yellow; break;
            case 6 : m_value = sf::Color::Magenta; break;
            case 7 : m_value = sf::Color::Cyan; break;
        }
        m_value.a = alpha;
    }
    ~Color() { SPDLOG_INFO("~Color()"); }

    sf::Color m_value;

};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_COLOR_HPP__