#ifndef __COMPONENTS_GRAVEYARD_HPP__
#define __COMPONENTS_GRAVEYARD_HPP__

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <entt/entity/registry.hpp>
#include <spdlog/spdlog.h>

namespace CelestialBodies::Components {

// This is a drawable component. 
class GraveYard {
public:

    GraveYard(std::shared_ptr<sf::RenderWindow> win ) 
    :   
        m_win(win),
        m_render_texture( m_win->getSize() )
    {}

    sf::Sprite get_sprite() { return sf::Sprite(m_render_texture.getTexture()); }

    void render() { m_win->draw(sf::Sprite(m_render_texture.getTexture())); }

    void bury(const sf::Drawable& drawable) 
    {
        m_render_texture.draw(drawable);
        m_render_texture.display();
    }

private:
    std::shared_ptr<sf::RenderWindow> m_win;
    sf::RenderTexture m_render_texture;
};

} // namespace CelestialBodies::Components

#endif // __COMPONENTS_GRAVEYARD_HPP__