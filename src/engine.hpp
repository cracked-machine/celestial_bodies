#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Angle.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <collision_system.hpp>

#include <entt/entity/fwd.hpp>
#include <graveyard.hpp>
#include <memory>
#include <planet.hpp>
#include <status.hpp>
#include <vector>

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

#include <spdlog/spdlog.h>

#include <entt/entity/registry.hpp>

#include <components/color.hpp>
#include <components/orbit.hpp>

#include <systems/base_system.hpp>
#include <systems/render_system.hpp>
#include <systems/trajectory_system.hpp>

namespace CelestialBodies {

class Engine {
public:
    Engine();

    // the main game loop
    bool run();

    sf::Font m_font = sf::Font("res/tuffy.ttf");

private:
    // SFML Window
    std::shared_ptr<sf::RenderWindow> m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode({1920u, 1080u}), "CelestialBodies");
    
    // ECS Registry
    entt::basic_registry<entt::entity> m_reg;

    //  ECS Systems
    std::unique_ptr<Systems::RenderSystem> m_render_sys = std::make_unique<Systems::RenderSystem> (m_window);
    std::unique_ptr<Systems::TrajectorySystem> m_trajectory_sys = std::make_unique<Systems::TrajectorySystem>();
    std::unique_ptr<Systems::CollisionSystem> m_collision_sys = std::make_unique<Systems::CollisionSystem>();

    std::vector<float> orbital_radius_samples{};
    
    void draw_stats_overlay(sf::Vector2f position, int text_size );

    // Update the Orbit component of each entity depending on their Status(ALIVE,DORMANT,EXTINCT)
    // Note: TrajectorySystem::update_cb() is listening for any Orbital component updates
    //  - Status:ALIVE:    entities have their orbital position incremented
    //  - Status:DORMANT:  entities spawn two Orbital children (if space allows) and then go extinct
    //  - Status:EXTINCT:  entities are consigned to the graveyard and then deleted.
    void update_orbits();

    // creates an entity with color, orbit and position components
    void add_body(
        int planet_radius = 0, 
        int requested_orbit_radius = 0, 
        int requested_start_point = 0,
        sf::Color requested_color = sf::Color::Transparent
    );

};

} //namespace CelestialBodies

#endif // __ENGINE_HPP__