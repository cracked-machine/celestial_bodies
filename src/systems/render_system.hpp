#ifndef __SYSTEMS_RENDER_SYSTEM_HPP__
#define __SYSTEMS_RENDER_SYSTEM_HPP__

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window/Window.hpp>

#include <spdlog/spdlog.h>

#include <memory>

#include <color.hpp>
#include <orbit.hpp>
#include <planet.hpp>
#include <status.hpp>
#include <systems/base_system.hpp>

namespace CelestialBodies::Systems {

class RenderSystem : public BaseSystem {
public:
    RenderSystem( std::shared_ptr<sf::RenderWindow> window ) : m_window( window ) 
    {   

        
    }

    ~RenderSystem() = default;
    void update() override {}

    // listener for Components::Planet updates
    void update_cb( entt::registry &reg, entt::entity entt )
    {
        using namespace Components;
        SPDLOG_DEBUG("RenderSystem update_cb");
        
        // draw body related item, if alive
        if( reg.get<Status>(entt)() == Status::State::ALIVE )
        {
            // draw orbital rings
            m_window->draw( reg.get<Orbit>(entt).orbit() );       

            // draw planet
            reg.get<Planet>( entt ).setFillColor( reg.get<Color>( entt ).value() );
            m_window->draw( reg.get<Planet>( entt ) );    

            // draw planet's orbit label
            radius_label.setPosition({ 
                reg.get<Planet>( entt ).getPosition().x - 20, 
                reg.get<Planet>( entt ).getPosition().y - 20
            });
            radius_label.setString( std::to_string( static_cast<int>(reg.get<Orbit>( entt ).get_radius())) );
            m_window->draw( radius_label );
            


            // draw planet's entity id on the planet
            entt_label.setFillColor( 
                (reg.get<Color>( entt ).value() == sf::Color::White || 
                reg.get<Color>( entt ).value() == sf::Color::Yellow || 
                reg.get<Color>( entt ).value() == sf::Color::Cyan) ? 
                sf::Color::Black : 
                sf::Color::White );

            entt_label.setPosition({
                    reg.get<Planet>( entt ).getPosition().x + reg.get<Planet>( entt ).getRadius(),
                    reg.get<Planet>( entt ).getPosition().y + reg.get<Planet>( entt ).getRadius()
                });
            m_window->draw(entt_label);
        }

       
    }
private:
    std::shared_ptr<sf::RenderWindow> m_window;
    sf::Font font = sf::Font("res/tuffy.ttf");
    sf::Text radius_label = sf::Text(font, "", 20);
    sf::Text entt_label = sf::Text(font, "", 20);
    
    sf::RenderTexture m_stats_window_texture;
    
};

} // namespace CelestialBodies::Systems

#endif // __SYSTEMS_RENDER_SYSTEM_HPP__