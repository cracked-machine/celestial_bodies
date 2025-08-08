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
#include <spdlog/spdlog.h>
#include <systems/base_system.hpp>

namespace CelestialBodies::Systems {

class RenderSystem : public BaseSystem {
public:
    RenderSystem(std::shared_ptr<sf::RenderWindow> window) : m_window(window)
    {}
    // RenderSystem() = default;
    ~RenderSystem() = default;
    void update() override {}

    void update_cb(entt::registry &registry, entt::entity entity)
    {
        SPDLOG_DEBUG("RenderSystem update_cb");
        
        sf::CircleShape body_sprite(5);
        body_sprite.setPosition( registry.get<Components::Position>(entity) );
        body_sprite.setFillColor( registry.get<Components::Color>(entity).m_value );
        m_window->draw(body_sprite);       
        
        // auto orbit = registry.get<Components::Orbit>(entity);
        // sf::CircleShape orbit_sprite(orbit.m_orbit.getRadius());
        // orbit_sprite.setPosition(orbit.m_orbit.getPosition());
        // orbit_sprite.setOutlineColor(color.m_value);
        // orbit_sprite.setOutlineThickness(1);
        // orbit_sprite.setFillColor(sf::Color::Transparent);
        // m_window->draw(orbit_sprite);       
       
    }
private:
    std::shared_ptr<sf::RenderWindow> m_window;
};

} // namespace CelestialBodies::Systems

#endif // __SYSTEMS_RENDER_SYSTEM_HPP__