#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <collision_system.hpp>
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
    Engine() {
        using namespace Components;
        m_window->setFramerateLimit(144);

        // register system listeners
        m_registry.on_update<Orbit>().connect<&Systems::TrajectorySystem::update_cb>(m_trajectory_sys);
        m_registry.on_update<Planet>().connect<&Systems::RenderSystem::update_cb>(m_render_sys);
        m_registry.on_update<Planet>().connect<&Systems::CollisionSystem::update_cb>(m_collision_sys);

        // create some bodies!
        for( auto i : std::vector<int>(30) ) { add_body(); }
        dump_sample_bins();
    }

    bool run()
    {
        using namespace Components;

        auto font = sf::Font("res/tuffy.ttf");
        auto text = sf::Text(font, "");
        std::string entity_string{"Enitity Count: "};
        sf::RenderTexture background_texture{m_window->getSize()};
        
        
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

            // draw this first, in the background
            sf::Sprite background_sprite(background_texture.getTexture());
            m_window->draw(background_sprite);

            // get any entities that have orbit components and update their position
            for( auto [ _entt,  _orbit, _status, _planet] : m_registry.view<Orbit, Status, Planet>().each() )
            {
                // TrajectorySystem is listening for updates
                if( m_registry.get<Status>(_entt)() == Status::State::ALIVE )
                {
                    m_registry.patch<Orbit>(_entt, [&](auto &orbit) { orbit++; });
                }
                else if ( m_registry.get<Status>(_entt)() == Status::State::DORMANT )
                {
                    // if the planet becomes dust then the it can have no more children
                    if( (m_registry.get<Planet>(_entt).getRadius() / 2) > 1 )
                    {
                        // spawm two child entities on the adjacent orbits, half the size of their parent
                        // auto nearest_adj_orbits = Orbit::get_nearest_to( m_registry.get<Orbit>(_entt).get_radius() );
                        add_body(
                            m_registry.get<Planet>(_entt).getRadius() / 2, 
                            // nearest_adj_orbits.first,
                            m_registry.get<Orbit>(_entt).get_point()    // dead parent orbit point
                        );
                        add_body( 
                            m_registry.get<Planet>(_entt).getRadius() / 2,
                            // nearest_adj_orbits.second, 
                            m_registry.get<Orbit>(_entt).get_point()    // dead parent orbit point
                        );
                        // dump_sample_bins();
                    }
                    m_registry.patch<Status>(_entt, [&](auto &status){ status.set(Status::State::EXTINCT); });
                                        
                }    
                else
                {
                    // we want to draw the extinct planets to the background 
                    // but we don't want to track all the entities       

                    m_registry.get<Planet>( _entt ).setFillColor( sf::Color(76,0,153, 128) );
                    
                    // don't clear the texture
                    background_texture.draw( m_registry.get<Planet>( _entt ) );
                    background_texture.display();

                    // remove it from our registry so we dont have to process it anymore
                    m_registry.destroy(_entt);
                }
            }
            text.setString(entity_string + std::to_string(m_registry.view<Planet>().size()));
            m_window->draw(text);
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
    std::unique_ptr<Systems::CollisionSystem> m_collision_sys = std::make_unique<Systems::CollisionSystem>();

    std::vector<float> orbital_radius_samples{};

    // creates an entity with color, orbit and position components
    void add_body(int planet_radius = 0, int orbit_radius = 0, int start_point = 0)
    {
        using namespace Components;
        auto entt = m_registry.create();
        
        // add orbit component, if we don't specify start point it will gen a random one
        m_registry.emplace<Orbit>(entt, m_window->getSize().x, m_window->getSize().y, start_point );
        orbital_radius_samples.push_back(m_registry.get<Orbit>(entt).get_radius());

        // add color component
        m_registry.emplace<Color>(entt);
        m_registry.emplace<Status>(entt);

        if( planet_radius ) { m_registry.emplace<Planet>( entt, planet_radius ); }
        else { m_registry.emplace<Planet>( entt ); }           
    }

    void dump_sample_bins()
    {
        using namespace Components;
        SPDLOG_INFO("Radius Bins size = {}", Orbit::radius_bins().size());
        std::stringstream out;
        for( auto &[key, value] : Orbit::radius_bins() )
        {
            out << key << " " << std::string(value, '*') << "\n";
        }        
        SPDLOG_INFO(out.str());

    }
};

} //namespace CelestialBodies

#endif // __ENGINE_HPP__