#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

namespace sf {
namespace plot {

class Grid : public Drawable, public Transformable {
private:
    static const int kPart = 8;
public:
    Grid();
    void setup(const Vector2f& size);
private:
    void draw(RenderTarget& target, RenderStates states) const;

    /// grid lines
    VertexArray vertices_;

    /// grid borders
    RectangleShape border_;

    /// grid color
    Color color_;

    /// keep the size
    Vector2f size_;
};

}
}