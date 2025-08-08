#ifndef __SYSTEMS_POSITION_SYSTEM_HPP__
#define __SYSTEMS_POSITION_SYSTEM_HPP__

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Window.hpp>
#include <components/position.hpp>
#include <entt/entity/fwd.hpp>

#include <components/orbit.hpp>
#include <spdlog/spdlog.h>
#include <systems/base_system.hpp>

namespace CelestialBodies::Systems {

class TrajectorySystem : public BaseSystem {
public:
    TrajectorySystem() = default;    
    ~TrajectorySystem() = default;

    void update_cb(entt::registry &registry, entt::entity entity)
    {
        if (not registry.all_of<Components::Position, Components::Orbit>(entity) )
        {
            return;
        }
        // RenderSystem is listening for updates
        registry.patch<Components::Position>(entity, 
            [&](auto &entity_pos) 
            {  
                entity_pos = registry.get<Components::Orbit>(entity).pos(); 
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