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

        // create some bodies!
        for( auto i : std::vector<int>(50) ) { add_body(); }
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

    // creates an entity with color, orbit and position components
    void add_body()
    {
        auto entt = m_registry.create();
        
        // add orbit component
        m_registry.emplace<CelestialBodies::Components::Orbit>(
            entt, 
            m_window->getSize().x, 
            m_window->getSize().y
        );

        // add color component
        m_registry.emplace<CelestialBodies::Components::Color>(entt);
        
        // add position component
        m_registry.emplace<CelestialBodies::Components::Position>(entt, 0.f, 0.f); 
           
    }
};

} //namespace CelestialBodies

#endif // __ENGINE_HPP__