#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Angle.hpp>
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
        m_window->setFramerateLimit(60);

        // register system listeners
        m_registry.on_update<Orbit>().connect<&Systems::TrajectorySystem::update_cb>(m_trajectory_sys);
        m_registry.on_update<Planet>().connect<&Systems::RenderSystem::update_cb>(m_render_sys);
        m_registry.on_update<Planet>().connect<&Systems::CollisionSystem::update_cb>(m_collision_sys);

        // create some bodies!
        for( auto i : std::vector<int>(10) ) { add_body(); }

    }

    bool run()
    {
        using namespace Components;

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

                update_entities( background_texture );
                background_texture.display();
                
                text.setString(entity_string + std::to_string(m_registry.view<Planet>().size()));
                m_window->draw(text);

                draw_stats_overlay( {20, 50}, 20 );
                

            m_window->display();
        }
        return false;   

    }

    sf::Font font = sf::Font("res/tuffy.ttf");
    sf::Text dead_entt_label = sf::Text(font, "", 20);
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

    void draw_stats_overlay(sf::Vector2f position, int text_size )
    {
        using namespace Components;
        float vertical_label_spacing = 1.5; 
        int count = 1;
        for( auto [ _entt,  _orbit, _planet, _status] : 
            m_registry.view<Orbit, Planet, Status>().each() )
        {
            /// STATS WINDOW
            // draw planet's entity id in the stats window
            sf::Text label( 
                font, 
                std::to_string(entt::entt_traits<entt::entity>::to_entity(_entt))
                    + " : " 
                    + std::to_string(_orbit.get_radius()) 
                    + " ( " 
                    + _status.to_string(_status()) 
                    + " )",
                text_size
            );
            label.setPosition({ position.x, position.y + (count * label.getLocalBounds().size.y * vertical_label_spacing) });
            m_window->draw(label);
            count++;
        }
    }

    void update_entities(sf::RenderTexture &background_texture)
    {
        using namespace Components;

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

                // spawm two child entities on the adjacent orbits, half the size of their parent
                // if the planet will become dust then it can have no more children
                if( (m_registry.get<Planet>(_entt).getRadius() / 2) > Planet::PLANET_RADIUS_MIN )
                {
                    // add a new child planet on a new inner orbit
                    auto new_inner_orbit = m_registry.get<Orbit>(_entt).get_radius() 
                            - (m_registry.get<Planet>(_entt).getRadius() * 2);

                    if( new_inner_orbit > Orbit::MIN_ORBIT_RADIUS )
                    {
                        add_body(
                            m_registry.get<Planet>(_entt).getRadius() / 2,
                            new_inner_orbit,
                            m_registry.get<Orbit>(_entt).get_point() - 20,
                            m_registry.get<Planet>( _entt ).getFillColor()
                        );
                    }
            
                    // add a new child planet on a new outer orbit
                    auto new_outer_orbit = m_registry.get<Orbit>(_entt).get_radius() 
                            + (m_registry.get<Planet>(_entt).getRadius() * 2);
                    if( new_outer_orbit < Orbit::MAX_ORBIT_RADIUS )
                    {
                        add_body(
                            m_registry.get<Planet>(_entt).getRadius() / 2,
                            new_outer_orbit,
                            m_registry.get<Orbit>(_entt).get_point() - 20,
                            m_registry.get<Planet>( _entt ).getFillColor()
                        );
                    }
                }
         
                // mark the parent as extinct
                m_registry.patch<Status>(_entt, [&](auto &status){ status.set(Status::State::EXTINCT); });
                                    
            }    
            else
            {
                // we want to draw the extinct planets to the background 
                // but we don't want to track all the entities       

                m_registry.get<Planet>( _entt ).setFillColor( sf::Color(76,0,153, 128) );
                background_texture.draw( m_registry.get<Planet>( _entt ) );

                // dead_entt_label.setPosition(m_registry.get<Planet>( _entt ).getPosition());
                // dead_entt_label.setString( std::to_string(entt::entt_traits<entt::entity>::to_entity(_entt)) );
                // dead_entt_label.setFillColor( sf::Color::White );
                // background_texture.draw( dead_entt_label );

                // remove it from our registry so we dont have to process it anymore
                m_registry.destroy(_entt);
            }
        }
    }

    // creates an entity with color, orbit and position components
    void add_body(
        int planet_radius = 0, 
        int requested_orbit_radius = 0, 
        int requested_start_point = 0,
        sf::Color requested_color = sf::Color::Transparent
    )
    {
        using namespace Components;
        auto entt = m_registry.create();
        
        // add orbit component, if we don't specify start point it will gen a random one
        m_registry.emplace<Orbit>(entt, m_window->getSize().x, m_window->getSize().y, requested_orbit_radius, requested_start_point );

        // add color component
        if( requested_color == sf::Color::Transparent ) { m_registry.emplace<Color>(entt); }
        else { m_registry.emplace<Color>(entt, requested_color); }

        m_registry.emplace<Status>(entt);

        if( planet_radius ) { m_registry.emplace<Planet>( entt, planet_radius ); }
        else { m_registry.emplace<Planet>( entt ); }           
    }

};

} //namespace CelestialBodies

#endif // __ENGINE_HPP__