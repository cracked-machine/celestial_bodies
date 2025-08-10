#ifndef __SYSTEMS_COLLISION_SYSTEM_HPP__
#define __SYSTEMS_COLLISION_SYSTEM_HPP__

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>

#include <spdlog/spdlog.h>

#include <planet.hpp>
#include <status.hpp>
#include <components/color.hpp>
#include <components/orbit.hpp>
#include <systems/base_system.hpp>

namespace CelestialBodies::Systems {

class CollisionSystem : public BaseSystem {
public:
    CollisionSystem() {}
    ~CollisionSystem() = default;
    void update() override {}

    // listener for Components::Planet updates
    void update_cb( entt::registry &reg, entt::entity entt )
    {
        using namespace Components;
        // get all components
        for( auto [ _entt,  _status, _planet, _orbit] : 
            reg.view< Status, Planet, Orbit>().each() )
        {
            // dont check for collision with self or with dead things
            if( 
                ( entt != _entt) && 
                ( reg.get<Status>(_entt)() == Status::State::ALIVE ) && 
                ( reg.get<Status>(entt)() == Status::State::ALIVE ) 
            )
            {
                auto collision = 
                    reg.get<Planet>(entt).getGlobalBounds().findIntersection(
                        _planet.getGlobalBounds()
                    );

                if( collision )
                {
                    // collision
                    if( reg.get<Planet>(entt).getRadius() < reg.get<Planet>(_entt).getRadius() ) 
                    {
                        reg.patch<Status>(entt, [&](auto &status) { status.set( Status::State::DORMANT ); });
                    }
                    else
                    {
                        reg.patch<Status>(_entt, [&](auto &status) { status.set( Status::State::DORMANT ); });
                    }

                    SPDLOG_DEBUG("Entity {} [{},{}] collided with {} [{},{}]", 
                        entt::entt_traits<entt::entity>::to_entity(entt),
                         reg.get<Planet>(entt).getGlobalBounds().getCenter().x,
                         reg.get<Planet>(entt).getGlobalBounds().getCenter().y,
                        entt::entt_traits<entt::entity>::to_entity(_entt),
                        _planet.getGlobalBounds().getCenter().x,
                        _planet.getGlobalBounds().getCenter().y

                    );
                }
            }
       
        }
    }
private:
   
};

} // namespace CelestialBodies::Systems

#endif // __SYSTEMS_COLLISION_SYSTEM_HPP__