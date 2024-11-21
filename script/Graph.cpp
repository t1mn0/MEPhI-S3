#include "Graph.hpp"

GraphGrid::GraphGrid(const std::string& Title, const std::string& xTitle, const std::string& yTitle, const sf::Vector2i& location) {
    plot_.setSize(sf::Vector2f(800, 600));
    plot_.setTitle(Title);
    plot_.setFont("../resources/font.ttf");
    plot_.setXLabel(xTitle);
    plot_.setYLabel(yTitle);
    plot_.setBackgroundColor(sf::Color(240, 255, 255));
    plot_.setTitleColor(sf::Color::Black);
    plot_.setPosition(sf::Vector2f(800 * location.x, 600 * location.y));
    sf::plot::Curve& curve = plot_.createCurve("graph", sf::Color(106, 90, 205));
    curve.setFill(16);
    curve.setThickness(5);
    curve.setColor(sf::Color(106, 90, 205));
    curve.setLimit(1000);
}

void GraphGrid::addValue(int value){
    sf::plot::Curve& curve = plot_.getCurve("graph");
    curve.addValue((float) value);
    plot_.prepare();
}

void GraphGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(plot_, states);
}
