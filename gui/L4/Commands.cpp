#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <cstdlib>

#include "GraphView.hpp"

namespace tmn {
namespace graph {

using Matrix = tmn::sequence::ArraySequence<tmn::sequence::ArraySequence<int>>;

void pretty_print_matrix(const Matrix& matrix, const tmn::sequence::ArraySequence<int>& seq){
    std::cout << "   ";
    for (auto& id : seq) {
        std::cout << id << "  ";
    }
    std::cout << std::endl;

    std::size_t k = 0;
    for (auto& i : matrix){
        std::cout << seq[k] << " ";
        for (auto& j : i){
            if (j >= 0) {
                std::cout << " " << j << " ";
            }
            else{
                std::cout << j << " ";
            }
        }
        ++k;
        std::cout << std::endl;
    }
}

void GraphView::init_complete_graph(int v_size) noexcept {
    regular_graph = make_complete_graph<false, int, int, double>(v_size, 0, 0.0);
}

void GraphView::init_bipartite_complete_graph(int part1, int part2) noexcept {
    try{
        regular_graph = make_bipartite_complete_graph<false, int, int, double>(part1, part2, 0, part1, 0.0);
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::init_simple_chain_graph(int v_size) noexcept {
    regular_graph = make_simple_chain_graph<false, int, int, double>(v_size, 0, 0.0);
}

void GraphView::init_wheel_graph(int v_size) noexcept {
    regular_graph = make_wheel_graph<false, int, int, double>(v_size, v_size / 2, 0, 0.0);
}

void GraphView::init_biconnected_graph(int v_size) noexcept {
    regular_graph = make_biconnected_graph<false, int, int, double>(v_size, 0, 0.0);
}

void GraphView::add_vertex(int v_id) noexcept {
    try {
        regular_graph.add_vertex(v_id);
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::remove_vertex(int v_id) noexcept {
    try {
        regular_graph.remove_vertex(v_id);
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::connected_vertices(int v_id) const noexcept {
    try {
        auto container = regular_graph.connected_vertices(v_id);
        std::cout << "Neighboring vertices of the graph for " + std::to_string(v_id) << std::endl;
        for (const auto& item : container){
            std::cout << item  << " ";
        }
        std::cout << std::endl;
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::positive_vertex_degree(int v_id) const noexcept {
    try {
        std::cout << regular_graph.positive_vertex_degree(v_id) << std::endl;
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::add_edge(int from, int to, double weight) noexcept {
    try {
        regular_graph.add_edge(from, to, weight);
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::remove_edge(int from, int to) noexcept {
    try {
        regular_graph.remove_edge(from, to);
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::add_connected_vertices(int from, int to, double weight) noexcept {
    try {
        regular_graph.add_connected_vertices(from, to, weight);
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::adjacency_list() const noexcept {
    try {
        std::cout << "Adjacency list:" << std::endl;
        auto pair = regular_graph.basic_adjacency_list();
        pretty_print_matrix(pair.first, pair.second);
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::has_cycle() const noexcept {
    try {
        if (regular_graph.has_cycle().has_value()){
            auto cycle = regular_graph.has_cycle().value();
            std::cout << "Graph has a cycle:" << std::endl; 
            for (const auto& item : cycle){
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
        else{
            std::cout << "Graph does not contain cyclic paths" << std::endl;
        }
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::is_connected_graph() const noexcept {
    try {
        if (regular_graph.is_connected_graph()){
            std::cout << "Graph is connected" << std::endl;
        } 
        else{
            std::cout << "Graph is not connected" << std::endl;
        }
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::coloring() const noexcept {
    try {
        auto colortable = regular_graph.graph_coloring();
        std::string dot_string = regular_graph.to_dot(colortable);

        const std::string dot_path = std::string(EXECUTABLE_PATH) + "/GraphViz/colorgraph.dot";
        const std::string png_path = std::string(EXECUTABLE_PATH) + "/GraphViz/colorgraph.png";

        if (std::filesystem::exists(dot_path)) {
            std::filesystem::remove(dot_path);
        }

        std::ofstream dot_file(dot_path);

        if (dot_file.is_open()) {
            dot_file << dot_string;
            dot_file.close();
        } else {
            std::cerr << "Error: Unable to open file for writing: " << dot_path << std::endl;
            return;
        }
        
        std::string command = "dot -Tpng \"" + dot_path + "\" -o \"" + png_path + "\"";

        int result = std::system(command.c_str());
        if (result != 0) {
            std::cerr << "Error: Graphviz command failed with code: " << result << std::endl;
            return;
        }
        #ifdef _WIN32
            std::string open_command = "start \"" + png_path + "\"";
        #elif __APPLE__
            std::string open_command = "open \"" + png_path + "\"";
        #elif __linux__
            std::string open_command = "xdg-open \"" + png_path + "\"";
        #else
            std::string open_command = "";
        std::cerr << "Error: unknown OS to open the file" << std::endl;
            return;
        #endif
        if (!open_command.empty()){
            std::system(open_command.c_str());
        }
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::find_shortest_path(int from, int to) const noexcept {
    try {
        auto container = regular_graph.find_shortest_path(from, to);
        if (container.empty()){
            std::cout << "Path not found" << std::endl;
        }
        else{
            for (const auto& item : container){
                std::cout << item << " ";
            }
            std::cout << std::endl;
        }
    }
    catch(tmn::exception::LogicException& e){
        std::cerr << e.what() << std::endl;
    }
}

void GraphView::update_visualization() const noexcept {
    std::string dot_string = regular_graph.to_dot();

    const std::string dot_path = std::string(EXECUTABLE_PATH) + "/GraphViz/graph.dot";
    const std::string png_path = std::string(EXECUTABLE_PATH) + "/GraphViz/graph.png";

    if (std::filesystem::exists(dot_path)) {
        std::filesystem::remove(dot_path);
    }

    std::ofstream dot_file(dot_path);

    if (dot_file.is_open()) {
        dot_file << dot_string;
        dot_file.close();
    } else {
        std::cerr << "Error: Unable to open file for writing: " << dot_path << std::endl;
        return;
    }
    
    std::string command = "dot -Tpng \"" + dot_path + "\" -o \"" + png_path + "\"";

    int result = std::system(command.c_str());
    if (result != 0) {
        std::cerr << "Error: Graphviz command failed with code: " << result << std::endl;
        return;
    }
    #ifdef _WIN32
        std::string open_command = "start \"" + png_path + "\"";
    #elif __APPLE__
        std::string open_command = "open \"" + png_path + "\"";
    #elif __linux__
        std::string open_command = "xdg-open \"" + png_path + "\"";
    #else
        std::string open_command = "";
    std::cerr << "Error: unknown OS to open the file" << std::endl;
        return;
    #endif
    if (!open_command.empty()){
        std::system(open_command.c_str());
    }
}

}
}