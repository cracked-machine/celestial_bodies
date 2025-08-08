#ifndef __COMPONENTS_RADIUS_HPP__
#define __COMPONENTS_RADIUS_HPP__

#include <SFML/System/Vector2.hpp>
#include <entt/entity/registry.hpp>
#include <random>
#include <spdlog/spdlog.h>

namespace CelestialBodies::Components {

// if param == 0 then `radius` = 5
//
// if param > 0 then `radius` is between 5 and 5 + r 
class Radius 
{
public:

    Radius(float r = 0) : m_size_dist(5, 5 + r)
    {
        value = m_size_dist(m_gen); 
    }
    virtual ~Radius() { SPDLOG_INFO("~Radius()"); }

    float value{};
private:
    static inline std::mt19937 m_gen{std::random_device{}()};
    std::uniform_int_distribution<> m_size_dist;
};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_RADIUS_HPP__