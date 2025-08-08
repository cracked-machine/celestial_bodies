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
    TrajectorySystem(entt::registry &registry)
    {
        // m_orbit_pool.bind(registry);
        // m_orbit_pool.on_construct<Components::Orbit>();
        // m_position_pool.bind(registry);
        // m_position_pool.on_construct<Components::Position>();

    }
    
    ~TrajectorySystem() = default;

    void update_cb(entt::registry &registry, entt::entity entity)
    {
        SPDLOG_INFO("TrajectorySystem update");
        auto pos = registry.get<Components::Position>(entity);
        auto orbit = registry.get<Components::Orbit>(entity);
        auto orbital_point = orbit.get(orbit.m_point);
        SPDLOG_INFO("{},{}", orbital_point.x, orbital_point.y);
        registry.patch<Components::Position>(entity, [&](auto &pos) 
        { 
            pos.x = orbital_point.x; 
            pos.y = orbital_point.y; 
        });
    
    }
    void update() override {
        // auto v = entt::basic_view{ m_orbit_pool, m_position_pool };
        
        // for(auto [entity, result, orbit]: m_planets_pool.view<Components::Orbit>().each()) {
        // for( auto e : entt::basic_view{ m_orbit_pool, m_position_pool } )
        //     // orbit.m_point++;
        // }
        // update positon of entity with using its orbit trajectory data

    }

private:
    // entt::reactive_mixin<entt::storage<void>> m_orbit_pool{};
    // entt::reactive_mixin<entt::storage<void>> m_position_pool{};
};

} // namespace CelestialBodies::Systems

#endif // __SYSTEMS_RENDER_SYSTEM_HPP__