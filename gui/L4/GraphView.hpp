#pragma once

#include <string>

#include "../../include/Graph/Graph.hpp"

namespace tmn {
namespace graph {

class GraphView {
private:
    Graph<false, int, int, double> regular_graph;
public:
    void run();

// Help :
    void global_help() const noexcept;

    void add_vertex_help() const noexcept;
    void remove_vertex_help() const noexcept;
    void connected_vertices_help() const noexcept;
    void positive_vertex_degree_help() const noexcept;
    void add_edge_help() const noexcept;
    void remove_edge_help() const noexcept;
    void add_connected_vertices_help() const noexcept;
    void adjacency_list_help() const noexcept;
    void has_cycle_help() const noexcept;
    void is_connected_graph_help() const noexcept;
    void coloring_help() const noexcept;
    void find_shortest_path_help() const noexcept;
    void update_visualization_help() const noexcept;
    
    void exit_help() const noexcept;

// Commands :
    void init_complete_graph(int v_size) noexcept;
    void init_bipartite_complete_graph(int part1, int part2) noexcept;
    void init_simple_chain_graph(int v_size) noexcept;
    void init_wheel_graph(int v_size) noexcept;
    void init_biconnected_graph(int v_size) noexcept;

    void add_vertex(int v_id) noexcept;
    void remove_vertex(int v_id) noexcept;
    void connected_vertices(int v_id) const noexcept;
    void positive_vertex_degree(int v_id) const noexcept;
    void add_edge(int from, int to, double weight) noexcept;
    void remove_edge(int from,int to) noexcept;
    void add_connected_vertices(int from, int to, double weight) noexcept;
    void adjacency_list() const noexcept;
    void has_cycle() const noexcept;
    void is_connected_graph() const noexcept;
    void coloring() const noexcept;
    void find_shortest_path(int from, int to) const noexcept;

    void update_visualization() const noexcept;

    void clear() const noexcept;
    void exit() const noexcept;
};

}
}