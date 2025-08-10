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

            // draw entity id associated with this Planet component
            entt_label.setPosition({ 
                reg.get<Planet>( entt ).getPosition().x - 20, 
                reg.get<Planet>( entt ).getPosition().y - 20
            });
            entt_label.setString( std::to_string( entt::entt_traits<entt::entity>::to_entity(entt) ) );
            m_window->draw( entt_label );

        }

       
    }
private:
    std::shared_ptr<sf::RenderWindow> m_window;
    sf::Font font = sf::Font("res/tuffy.ttf");

    sf::Text entt_label = sf::Text(font, "", 20);
    
    sf::RenderTexture m_stats_window_texture;
    
};

} // namespace CelestialBodies::Systems

#endif // __SYSTEMS_RENDER_SYSTEM_HPP__