#pragma once

#include <vector>
#include <list>
#include <string>
#include <SFML/Graphics.hpp>
#include "line.hpp"
#include "area.hpp"

namespace sf {
namespace plot {

class Curve : public sf::Drawable, public sf::Transformable {
public:
    Curve();
    Curve(const sf::Vector2f &size, const sf::Color& color);
    void setLabel(const std::string &label);
    void setColor(const sf::Color &color);
    void addValue(float value);
    void prepare(sf::Vector2f &rangex, sf::Vector2f &rangey);
    void setFill(bool fill);
    void setThickness(float thickness);
    void setLimit(unsigned int limit);
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /// curve label
    std::string label_;

    /// curve color
    sf::Color color_;

    /// cuve area color
    sf::Color fillColor_;

    /// curve data
    std::list<float> data_;

    /// curve size
    sf::Vector2f size_;

    /// plain ?
    bool fill_;

    /// line object
    Line line_;

    /// area object
    Area area_;

    /// thickness size
    float thickness_;

    /// limit of data
    unsigned int limit_;
};

inline void Curve::setLimit(unsigned int limit) {
    limit_ = limit;
}

inline void Curve::setThickness(float thickness) {
    thickness_ = thickness;
}

inline void Curve::setFill(bool fill) {
    fill_ = fill;
}

inline void Curve::setLabel(const std::string &label) {
    label_ = label;
}

inline void Curve::setColor(const sf::Color &color) {
    color_ = color;
    fillColor_ = color;
    fillColor_.a = 100;
}

}
}