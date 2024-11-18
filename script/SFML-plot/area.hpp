#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

namespace sf {
namespace plot {

class Area : public sf::Drawable, public sf::Transformable {
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::VertexArray vertices_;
public:
    Area();
    void add(const sf::Vector2f& point, const sf::Color& color = sf::Color(255, 255, 0), float size_y = 100);
    void clear();
};

}
}