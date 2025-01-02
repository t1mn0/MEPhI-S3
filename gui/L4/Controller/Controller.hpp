#pragma once

#include <string>
#include <cstdint>

#include <SFML/Graphics.hpp>

#include "../../../include/Optional/Optional.hpp"
#include "../../../include/Pair/Pair.hpp"
#include "../../../include/Graph/Graph.hpp"

namespace tmn { namespace visualize { namespace graph {

using unchar = unsigned char;

struct VertexResource {
    sf::Vector2f position;
    sf::Color color;
};

template <bool is_graph_oriented, bool has_weight> 
class ConsoleController {
private:
    bool is_graph_init = false; 
    Graph<is_graph_oriented, unchar, VertexResource, void> optional_graph;
public:
    ConsoleController() = default;
    ~ConsoleController() = default;

    unchar main_menu() const noexcept;
    unchar creating_graph_by_parameters_menu() const noexcept;
    unchar algorithms_menu() const noexcept;

    std::string get_graph_info() const noexcept;

    void creating_graph_by_parameters();
    void creating_random_graph();
    void delete_graph() noexcept;
};



template <bool is_graph_oriented> 
class ConsoleController<is_graph_oriented, true> {
private:
    bool is_init = false; 
    Graph<is_graph_oriented, unchar, VertexResource, unsigned int> optional_graph;
public:
    ConsoleController() = default;
    ~ConsoleController() = default;

    unchar main_menu() const noexcept;
    unchar creating_graph_by_parameters_menu() const noexcept;
    unchar algorithms_menu() const noexcept;

    std::string get_graph_info() const noexcept;

    Optional<Graph<is_graph_oriented, unchar, VertexResource, void>> creating_graph_by_parameters();
    Optional<Graph<is_graph_oriented, unchar, VertexResource, void>> creating_random_graph();
    void delete_graph() noexcept;
};

} } } // closing namespaces