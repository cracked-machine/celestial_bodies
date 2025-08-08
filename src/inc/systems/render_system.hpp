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
        SPDLOG_INFO("RenderSystem update_cb");
        auto pos = registry.get<Components::Position>(entity);
        auto color = registry.get<Components::Color>(entity);
        auto orbit = registry.get<Components::Orbit>(entity);

        sf::CircleShape body_sprite(5);
        body_sprite.setPosition(pos);
        body_sprite.setFillColor(color.m_value);

        // sf::CircleShape orbit_sprite(orbit.m_orbit.getRadius());
        // orbit_sprite.setPosition(orbit.m_orbit.getPosition());
        // orbit_sprite.setOutlineColor(color.m_value);
        // orbit_sprite.setOutlineThickness(1);
        // orbit_sprite.setFillColor(sf::Color::Transparent);


        m_window->clear();
        // m_window->draw(orbit_sprite);       
        m_window->draw(body_sprite);       
        m_window->display();
    }
private:
    std::shared_ptr<sf::RenderWindow> m_window;
};

} // namespace CelestialBodies::Systems

#endif // __SYSTEMS_RENDER_SYSTEM_HPP__