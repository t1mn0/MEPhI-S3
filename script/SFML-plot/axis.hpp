#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

namespace sf{
namespace plot{

class Axis : public sf::Drawable , public sf::Transformable {
private:
    static const int kPart = 4;
public:
    Axis();
    void setSize(float size);
    void setName(const std::string &name);
    void prepare(const Vector2f &range);
    void setFont(sf::Font *font);
private:
    void defineLabel();
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /// axis name
    std::string name_;

    /// global font
    sf::Font *font_;

    /// axis size
    float size_;

    /// axis label
    sf::Text label_;

    /// axis number
    std::vector<sf::Text> numbers_;
};

inline void Axis::setFont(sf::Font *font){
    font_ = font;
    defineLabel();
}

inline void Axis::setSize(float size){
    size_ = size;
}

inline void Axis::setName(const std::string &name){
    name_ = name;
    defineLabel();
}

}
}