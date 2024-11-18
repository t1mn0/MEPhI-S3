#pragma once

#include <SFML/Graphics.hpp>

namespace sf {
namespace plot {

class Line : public Drawable, public Transformable {
private:
    VertexArray vertices_;
    Vector2f previous_;
    unsigned int pwidth_;
    Color color_;
public:
    Line();
    void add(const Vector2f& point, unsigned int thickness = 2, const Color& color = Color(255, 255, 0));
    void clear();
    size_t size() const;
    void draw(RenderTarget &target, RenderStates states) const;
};

inline size_t Line::size() const {
    return vertices_.getVertexCount();
}

}
}