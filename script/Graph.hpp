#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <fstream>
#include <iostream>

#include "PLOT_SFML/plot.hpp"

class GraphGrid : public sf::Drawable {
private:
    sf::plot::Plot plot_;
public:
    GraphGrid(const std::string& Title, const std::string& xTitle, const std::string& yTitle,  const sf::Vector2i& location);
    void addValue(int value);
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};
