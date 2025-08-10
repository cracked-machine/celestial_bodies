#ifndef __SYSTEMS_POSITION_SYSTEM_HPP__
#define __SYSTEMS_POSITION_SYSTEM_HPP__

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>

#include <entt/entity/fwd.hpp>
#include <spdlog/spdlog.h>

#include <orbit.hpp>
#include <planet.hpp>
#include <systems/base_system.hpp>

namespace CelestialBodies::Systems {

class TrajectorySystem : public BaseSystem {
public:
    TrajectorySystem() = default;    
    ~TrajectorySystem() = default;

    void update_cb(entt::registry &registry, entt::entity entity)
    {
        using namespace Components;
        if (not registry.all_of<Orbit, Planet>(entity) )
        {
            return;
        }
        // RenderSystem is listening for updates
        registry.patch<Planet>(entity, 
            [&](auto &planet) 
            {  
                planet.setPosition( { 
                    registry.get<Orbit>(entity).pos().x - (planet.getRadius()),
                    registry.get<Orbit>(entity).pos().y - (planet.getRadius())
                }); 
            });    

        SPDLOG_DEBUG("TrajectorySystem update");
    }
    
    void update() override 
    {
    }

private:

};

} // namespace CelestialBodies::Systems

#endif // __SYSTEMS_RENDER_SYSTEM_HPP__