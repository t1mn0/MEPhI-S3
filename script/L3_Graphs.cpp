#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cstring> // memset
#include <fstream>
#include <iostream>

#include "SFML-plot/plot.hpp"

class GraphGrid : public sf::Drawable {
private:
    sf::plot::Plot plot_;
public:
    GraphGrid(const sf::Vector2i& location) {
        plot_.setSize(sf::Vector2f(1200, 800));
        plot_.setTitle("Graph-L3");
        plot_.setFont("../resources/font.ttf");
        plot_.setXLabel("Test search files by their path");
        plot_.setYLabel("Nanoseconds");
        plot_.setBackgroundColor(sf::Color(240, 255, 255));
        plot_.setTitleColor(sf::Color::Black);
        plot_.setPosition(sf::Vector2f(1200 * location.x, 800 * location.y));
        sf::plot::Curve& curve = plot_.createCurve("graph", sf::Color(106, 90, 205));
        curve.setFill(16);
        curve.setThickness(3);
        curve.setColor(sf::Color(106, 90, 205));
        curve.setLimit(10000);
    }

    void addValue(int value, bool prep){
        sf::plot::Curve& curve = plot_.getCurve("graph");
        curve.addValue((float) value);
        if (prep) {
            plot_.prepare();
        }
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        target.draw(plot_, states);
    }

};

std::vector<int>* SearchResults(){
    std::ifstream input_file("/home/timno/Documents/MEPhI-S3/test/_test_results/search_test_indextable.txt");

    std::vector<int>* values = new std::vector<int>;

    if (!input_file.is_open()) {
        throw std::runtime_error("Could not open file!");
    }
    
    std::string line;
    while (std::getline(input_file, line)) {
        size_t pos = line.find("took");
        if (pos != std::string::npos) {
            size_t start_pos = line.find(" ", pos) + 1;
            size_t end_pos = line.find(" ", start_pos);
            std::string num_str;

            if (end_pos != std::string::npos){
                num_str = line.substr(start_pos, end_pos - start_pos);
            } else {
                num_str = line.substr(start_pos);
            }
            try {
                int value = std::stoi(num_str);
                values->push_back(value);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Warning: Invalid number in line: " << line << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Warning: Number out of range in line: " << line << std::endl;
            }
        }
        if (values->size() >= 10000){
            break;
        }
    }

    input_file.close();

    return values;
}


int main(){
    srand (time(NULL));

    std::vector<int> values = *(SearchResults());

    sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(1200, 800), "(L3) Search in index-table", sf::Style::Default);

    GraphGrid graph(sf::Vector2i(0, 0));

    bool is_first = true;

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear();
        
        if (is_first){
            for (int i = 0; i < values.size(); ++i){
                if(clock.getElapsedTime().asMilliseconds() > 10) {
                    clock.restart();
                    graph.addValue(values[i], true);
                }

                window.draw(graph);
                window.display();
            }
        }
        is_first = false;
    }

    return 0;
}