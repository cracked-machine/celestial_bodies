#ifndef __SYSTEMS_RENDER_SYSTEM_HPP__
#define __SYSTEMS_RENDER_SYSTEM_HPP__

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>
#include <components/color.hpp>
#include <components/orbit.hpp>
#include <components/position.hpp>
#include <memory>
#include <radius.hpp>
#include <spdlog/spdlog.h>
#include <systems/base_system.hpp>

namespace CelestialBodies::Systems {

class RenderSystem : public BaseSystem {
public:
    RenderSystem( std::shared_ptr<sf::RenderWindow> window ) : m_window( window ) {}
    ~RenderSystem() = default;
    void update() override {}

    // listener for Components::Position updates
    void update_cb( entt::registry &registry, entt::entity entity )
    {
        if (not registry.all_of<Components::Position, Components::Orbit, Components::Color>(entity) )
        {
            return;
        }
        SPDLOG_DEBUG("RenderSystem update_cb");
        
        // draw orbital body
        sf::CircleShape body_sprite( registry.get<Components::Radius>( entity ).value );
        body_sprite.setPosition( {
            registry.get<Components::Position>( entity ).x - body_sprite.getRadius() / 2,
            registry.get<Components::Position>( entity ).y - body_sprite.getRadius() / 2 
        });
        body_sprite.setFillColor( registry.get<Components::Color>( entity ).value() );
        m_window->draw( body_sprite );       

        // draw orbital ring
        sf::CircleShape orbit_sprite(
            registry.get<Components::Orbit>(entity).get_radius(),
            registry.get<Components::Orbit>(entity).get_resolution()
        );
        orbit_sprite.setFillColor( sf::Color::Transparent );
        orbit_sprite.setOutlineColor( sf::Color(255, 255, 255, 16) );
        orbit_sprite.setOutlineThickness( 1 );
        orbit_sprite.setPosition( registry.get<Components::Orbit>(entity).get_center() );
        m_window->draw( orbit_sprite );       
       
    }
private:
    std::shared_ptr<sf::RenderWindow> m_window;
};

} // namespace CelestialBodies::Systems

#endif // __SYSTEMS_RENDER_SYSTEM_HPP__