#ifndef __SYSTEMS_RENDER_SYSTEM_HPP__
#define __SYSTEMS_RENDER_SYSTEM_HPP__

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
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
    RenderSystem( std::shared_ptr<sf::RenderWindow> window ) : m_window( window ) {}
    ~RenderSystem() = default;
    void update() override {}

    // listener for Components::Planet updates
    void update_cb( entt::registry &reg, entt::entity entt )
    {
        using namespace Components;
        SPDLOG_DEBUG("RenderSystem update_cb");
        
        // draw orbital body, if alive
        if( reg.get<Status>(entt)() == Status::State::ALIVE )
        {
            reg.get<Planet>( entt ).setFillColor( reg.get<Color>( entt ).value() );
            m_window->draw( reg.get<Planet>( entt ) );    
            m_window->draw( reg.get<Orbit>(entt).orbit() );       
        }
        else 
        {

        }
       
    }
private:
    std::shared_ptr<sf::RenderWindow> m_window;
    
};

} // namespace CelestialBodies::Systems

#endif // __SYSTEMS_RENDER_SYSTEM_HPP__