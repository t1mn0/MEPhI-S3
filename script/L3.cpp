#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <cstring>
#include <fstream>
#include <iostream>

#include "Graph.hpp"

std::vector<int> DirectSearchResults(){
    std::ifstream input_file("/home/timno/Documents/MEPhI-S3/test/_test_results/direct_search_test_indextable.txt");

    std::vector<int> values;

    if (!input_file.is_open()) {
        throw std::runtime_error("Could not open file : direct_search_test_indextable.txt");
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
                values.push_back(value);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Warning: Invalid number in line: " << line << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Warning: Number out of range in line: " << line << std::endl;
            }
        }
        if (values.size() >= 1250){
            break;
        }
    }

    input_file.close();

    return values;
}

std::vector<int> IndirectSearchResults(){
    std::ifstream input_file("/home/timno/Documents/MEPhI-S3/test/_test_results/indirect_search_test_indextable.txt");

    std::vector<int> values;

    if (!input_file.is_open()) {
        throw std::runtime_error("Could not open file : indirect_search_test_indextable.txt");
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
                values.push_back(value);
            } catch (const std::invalid_argument& e) {
                std::cerr << "Warning: Invalid number in line: " << line << std::endl;
            } catch (const std::out_of_range& e) {
                std::cerr << "Warning: Number out of range in line: " << line << std::endl;
            }
        }
        if (values.size() >= 1250){
            break;
        }
    }

    input_file.close();

    return values;
}

int main(){
    std::vector<int> direct_search_values = DirectSearchResults();
    std::vector<int> indirect_search_values = IndirectSearchResults();

    sf::RenderWindow window(sf::VideoMode(1600, 600), "(L3) Search in index-table", sf::Style::Default);

    GraphGrid* dir_search_graph = new GraphGrid("Direct search test FileDescriptors (by path)", "Test", "Nanoseconds", sf::Vector2i(0, 0));
    GraphGrid* indir_search_graph = new GraphGrid("Indirect search test FileDescriptors (by name)", "Test", "Microseconds", sf::Vector2i(1, 0));

    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }
        
        window.clear();
        
        for (int i = 0; i < direct_search_values.size(); ++i){
            dir_search_graph->addValue(direct_search_values[i]);
            indir_search_graph->addValue(indirect_search_values[i]);
        }

        window.draw(*dir_search_graph);
        window.draw(*indir_search_graph);
        window.display();
    }

    delete dir_search_graph;
    delete indir_search_graph;

    return 0;
}