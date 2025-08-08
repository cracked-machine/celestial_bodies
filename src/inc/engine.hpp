#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstddef>
#include <memory>
#include <random>

#include <SFML/Graphics.hpp>
#include <entt/entity/registry.hpp>

#include <components/color.hpp>
#include <components/orbit.hpp>
#include <components/position.hpp>

#include <spdlog/spdlog.h>
#include <systems/base_system.hpp>
#include <systems/render_system.hpp>
#include <systems/trajectory_system.hpp>
#include <vector>

namespace CelestialBodies {

class Engine {
public:
    Engine() {
        m_window->setFramerateLimit(144);

        // register system listeners
        m_registry.on_update<Components::Orbit>().connect<&Systems::TrajectorySystem::update_cb>(m_trajectory_sys);
        m_registry.on_update<Components::Position>().connect<&Systems::RenderSystem::update_cb>(m_render_sys);

        for( auto i : std::vector<int>(50) )
            add_body();

    }

    bool run()
    {
        while (m_window->isOpen())
        {
            while (const std::optional event = m_window->pollEvent())
            {
                if (event->is<sf::Event::Closed>())
                {
                    m_window->close();
                }
            }
            
            m_window->clear();
            // get any entities that have orbit components and update their position
            auto view = m_registry.view<Components::Orbit>()->each();
            for( auto [ entt,  orbit] : view )
            {
                // TrajectorySystem is listening for updates
                m_registry.patch<Components::Orbit>(entt, [&](auto &orbit) { orbit++; });
                SPDLOG_DEBUG("Updating entity #{} orbital position", entt::entt_traits<entt::entity>::to_entity(entt));
            }
             m_window->display();
        }
        return false;   
    }

private:
    // SFML Window
    std::shared_ptr<sf::RenderWindow> m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode({1920u, 1080u}), "CelestialBodies");
    
    // ECS Registry
    entt::basic_registry<entt::entity> m_registry;

    //  ECS Systems
    std::unique_ptr<Systems::RenderSystem> m_render_sys = std::make_unique<Systems::RenderSystem> (m_window);
    std::unique_ptr<Systems::TrajectorySystem> m_trajectory_sys = std::make_unique<Systems::TrajectorySystem>();
    
    // Random Seed
    std::random_device rd;
    
    void add_body()
    {
        using Orbit = CelestialBodies::Components::Orbit;
        using Color = CelestialBodies::Components::Color;
        using Position = CelestialBodies::Components::Position;
        
        std::mt19937 rng(rd());

        auto entt = m_registry.create();
        
        // add orbit component
        auto orbit_radius = std::uniform_real_distribution(10.f, 600.1f)(rng);
        sf::Vector2f orbit_center( 
            m_window->getSize().x * 0.5f - orbit_radius, 
            m_window->getSize().y * 0.5f - orbit_radius
        );
        auto points =  orbit_radius * 2;
        m_registry.emplace<Orbit>(entt, orbit_radius, points, orbit_center);

        // add color component
        int colorpick = std::uniform_int_distribution(1, 7)(rng);
        m_registry.emplace<Color>(entt, colorpick);
        
        // add position component
        m_registry.emplace<Position>(entt, 0.f, 0.f); 

           
    }
};

} //namespace CelestialBodies

#endif // __ENGINE_HPP__