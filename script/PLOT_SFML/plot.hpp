#pragma once

#include <map>
#include <string>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "axis.hpp"
#include "grid.hpp"
#include "curve.hpp"

namespace sf {
namespace plot {

class Plot : public sf::Drawable, public sf::Transformable{
public:
    Plot();
    Plot(const sf::Vector2f &size, const std::string &title);
    Curve &createCurve(const std::string &name, const Color &color);

    bool haveCurve(const std::string &name) const;
    Curve& getCurve(const std::string& name);
    void prepare();
    void setXLabel(const std::string &name);
    void setYLabel(const std::string &name);
    void setBackgroundColor(const sf::Color &color);
    void setTitleColor(const sf::Color &color);
    void setTitle(const std::string &title);
    void setFont(const std::string &filename);
    void setSize(const sf::Vector2f& size);
private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    /// x axis
    Axis xaxis_;

    /// y axis
    Axis yaxis_;

    /// grid isntance
    plot::Grid grid_;

    /// size of the chart
    sf::Vector2f size_;

    /// collection of curves
    std::map<std::string, Curve> curves_;

    /// graph size (reduce size from size)
    sf::Vector2f graphSize_;
    /// graph real position
    sf::Vector2f graphPos_;

    static const int kTopSize = 10;
    static const int kLeftSize = 12;
    static const int kRightSize = 5;
    static const int kBottomSize = 12;
    static const int kBorderSize = 2;

    /// global font
    sf::Font font_;

    /// title
    sf::Text title_;

    /// background
    sf::RectangleShape bg_;
};


inline void Plot::setXLabel(const std::string &name){
    xaxis_.setName(name);
}

inline void Plot::setYLabel(const std::string &name){
    yaxis_.setName(name);
}

inline void Plot::setTitleColor(const sf::Color &color){
    title_.setFillColor(color);
}

inline void Plot::setBackgroundColor(const sf::Color &color){
    bg_.setFillColor(color);
}

}
}