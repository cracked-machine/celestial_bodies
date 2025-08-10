#ifndef __COMPONENTS_RANDINT_HPP__
#define __COMPONENTS_RANDINT_HPP__

#include <random>
#include <base.hpp>

namespace CelestialBodies::Components {

class RandInt : public Components::Base {
public:
    RandInt(int min, int max) : m_intdist(min, max) { SPDLOG_DEBUG("RandInt()"); }
    ~RandInt() { SPDLOG_DEBUG("~RandInt()"); }
    int gen() { return m_intdist( m_randgen ); }

private:
    static inline std::mt19937 m_randgen{std::random_device{}()};
    std::uniform_int_distribution<> m_intdist;
};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_RANDINT_HPP__