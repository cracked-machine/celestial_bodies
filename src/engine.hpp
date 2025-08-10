#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/System/Angle.hpp>
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
    Engine() {
        using namespace Components;
        m_window->setFramerateLimit(30);

        // this is where EXTINCT entities go for retirement
        m_reg.emplace<GraveYard>(m_reg.create(), m_window);

        // register system listeners
        m_reg.on_update<Orbit>().connect<&Systems::TrajectorySystem::update_cb>(m_trajectory_sys);
        m_reg.on_update<Planet>().connect<&Systems::RenderSystem::update_cb>(m_render_sys);
        m_reg.on_update<Planet>().connect<&Systems::CollisionSystem::update_cb>(m_collision_sys);

        // create some bodies!
        for( auto i : std::vector<int>(10) ) { add_body(); }
        
    }

    bool run()
    {
        using namespace Components;
        
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
        
                for( auto [_, graveyard] : m_reg.view<GraveYard>().each() )
                {
                    graveyard.render();
                    m_window->draw(graveyard.get_sprite());
                }

                // this will trigger a callback chain:
                //                                    |-> (Planet update) RenderSystem 
                // (Orbit update) TrajectorySystem  --|
                //                                    |-> (Planet update) CollisionSystem
                update_orbits();            
                draw_stats_overlay( {20, 50}, 20 );
                
            m_window->display();
        }
        return false;   
        
    }

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
    
    void draw_stats_overlay(sf::Vector2f position, int text_size )
    {
        using namespace Components;
        float vertical_label_spacing = 1.5; 
        int count = 1;
        
        // display entity count
        sf::Text entity_count_text = sf::Text(m_font, "");
        std::string entity_string{"Enitity Count: "};
        entity_count_text.setString(entity_string + std::to_string(m_reg.view<Planet>().size()));
        m_window->draw(entity_count_text);

        for( auto [ _entt,  _orbit, _planet, _status] : 
            m_reg.view<Orbit, Planet, Status>().each() )
        {
            // display planet entity id's and their orbit radius
            sf::Text label( 
                m_font, 
                std::to_string(entt::entt_traits<entt::entity>::to_entity(_entt))
                    + " : " 
                    + std::to_string( static_cast<int>(_orbit.get_radius()) ) 
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

    // Update the Orbit component of each entity depending on their Status(ALIVE,DORMANT,EXTINCT)
    // Note: TrajectorySystem::update_cb() is listening for any Orbital component updates
    //  - Status:ALIVE:    entities have their orbital position incremented
    //  - Status:DORMANT:  entities spawn two Orbital children (if space allows) and then go extinct
    //  - Status:EXTINCT:  entities are consigned to the graveyard and then deleted.
    void update_orbits()
    {
        using namespace Components;

        // get any entities that have orbit components and update their position
        for( auto [ _entt,  _orbit, _status, _planet] : m_reg.view<Orbit, Status, Planet>().each() )
        {
            // TrajectorySystem is listening for updates
            if( m_reg.get<Status>(_entt)() == Status::State::ALIVE )
            {
                m_reg.patch<Orbit>(_entt, [&](auto &orbit) { orbit++; });
            }
            else if ( m_reg.get<Status>(_entt)() == Status::State::DORMANT )
            {

                // spawm two child entities on the adjacent orbits, half the size of their parent
                // if the planet will become dust then it can have no more children
                if( (m_reg.get<Planet>(_entt).getRadius() / 2) > Planet::PLANET_RADIUS_MIN )
                {
                    // add a new child planet on a new inner orbit
                    auto new_inner_orbit = m_reg.get<Orbit>(_entt).get_radius() 
                            - (m_reg.get<Planet>(_entt).getRadius() * 2);

                    if( new_inner_orbit > Orbit::MIN_ORBIT_RADIUS )
                    {
                        add_body(
                            m_reg.get<Planet>(_entt).getRadius() / 2,
                            new_inner_orbit,
                            m_reg.get<Orbit>(_entt).get_point() - 5,
                            m_reg.get<Planet>( _entt ).getFillColor()
                        );
                    }
            
                    // add a new child planet on a new outer orbit
                    auto new_outer_orbit = m_reg.get<Orbit>(_entt).get_radius() 
                            + (m_reg.get<Planet>(_entt).getRadius() * 2);
                    if( new_outer_orbit < Orbit::MAX_ORBIT_RADIUS )
                    {
                        add_body(
                            m_reg.get<Planet>(_entt).getRadius() / 2,
                            new_outer_orbit,
                            m_reg.get<Orbit>(_entt).get_point() - 5,
                            m_reg.get<Planet>( _entt ).getFillColor()
                        );
                    }
                }
         
                // mark the parent as extinct
                m_reg.patch<Status>(_entt, [&](auto &status){ status.set(Status::State::EXTINCT); });
                                    
            }    
            else // EXTINCT
            {
                // we want to draw the extinct planets to the background 
                // but we don't want to track all the entities       
                
                for( auto [_, graveyard] : m_reg.view<GraveYard>().each() )
                {
                    m_reg.get<Planet>( _entt ).setFillColor( sf::Color(76,0,153, 128) );
                    graveyard.bury(m_reg.get<Planet>( _entt ));
                }
                           
                // remove ExTINCT entity from our registry so we dont have to process it anymore
                m_reg.remove<Planet>(_entt);
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
        auto entt = m_reg.create();
        
        // add orbit component, if we don't specify start point it will gen a random one
        m_reg.emplace<Orbit>(entt, m_window->getSize(), requested_orbit_radius, requested_start_point );

        // add color component
        if( requested_color == sf::Color::Transparent ) { m_reg.emplace<Color>(entt); }
        else { m_reg.emplace<Color>(entt, requested_color); }

        m_reg.emplace<Status>(entt);

        if( planet_radius ) { m_reg.emplace<Planet>( entt, planet_radius ); }
        else { m_reg.emplace<Planet>( entt ); }           
    }

};

} //namespace CelestialBodies

#endif // __ENGINE_HPP__