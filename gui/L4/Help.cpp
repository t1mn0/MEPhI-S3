#include <iostream>
#include <string>

#include "GraphView.hpp"

namespace tmn {
namespace graph {

void GraphView::global_help() const noexcept{
    std::cout << "You are working with a directed graph whose vertices are 'int' numbers and whose weights are 'double' numbers" << std::endl;
    std::cout << "These are common commands used in various situations:" << std::endl;
	std::cout << "  - add_v : Adds a new vertex to the graph" << std::endl;
	std::cout << "  - rm_v  : Deletes the specified vertex in the graph" << std::endl;
	std::cout << "  - connected_v : Shows all the neighbors of the specified vertex" << std::endl;
	std::cout << "  - pos_degree : Shows the positive degree of the specified vertex" << std::endl;
	std::cout << "  - add_e : Adds an edge between the specified vertices with the specified weight" << std::endl;
	std::cout << "  - rm_e : Deletes an edge between the specified vertices" << std::endl;
	std::cout << "  - add_pair_v : Adds a connected vertex-vertex pair via an edge with the specified weight" << std::endl;
	std::cout << "  - adj_list : Shows an unordered adjacency list" << std::endl;
	std::cout << "  - has_cycle : Checks for a cycle in the graph" << std::endl;
	std::cout << "  - is_connected : Checks that the graph is connected" << std::endl;
	std::cout << "  - coloring : Runs the graph coloring algorithm" << std::endl;
	std::cout << "  - shortest_path : Finds the shortest path from one vertex to another in the graph" << std::endl;
    std::cout << "  - update_viz : Draws a graph on request" << std::endl;
    std::cout << "  - exit : Shutdown" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Use [help] [command] for a detailed description of the commands and their modifications " << std::endl;
	std::cout << "{} brackets indicate that parameter is optional" << std::endl;
	std::cout << "<> brackets indicate that parameter must be specified" << std::endl;
}

void GraphView::add_vertex_help() const noexcept {
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - add_v <vertex-id>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Adds a new vertex to the graph" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::remove_vertex_help() const noexcept {
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - rm_v <vertex-id>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Deletes the specified vertex in the graph" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::connected_vertices_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - connected_v <vertex-id>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Shows all the neighbors of the specified vertex" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::positive_vertex_degree_help() const noexcept{
std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - pos_degree <vertex-id>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Shows the positive degree of the specified vertex" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::add_edge_help() const noexcept{
std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - add_e <vertex-id> <vertex-id> {weight}" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Adds an edge between the specified vertices with the specified weight" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::remove_edge_help() const noexcept{
std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - rm_e <vertex-id> <vertex-id>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Deletes an edge between the specified vertices" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::add_connected_vertices_help() const noexcept{
std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - add_pair_v <vertex-id> <vertex-id> {weight}" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Adds a connected vertex-vertex pair via an edge with the specified weight" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::adjacency_list_help() const noexcept{
std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - adj_list" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Shows an unordered adjacency list" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::has_cycle_help() const noexcept{
std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - has_cycle" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Checks for a cycle in the graph" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::is_connected_graph_help() const noexcept{
std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - is_connected" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Checks that the graph is connected" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::coloring_help() const noexcept{
std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - coloring" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Runs the graph coloring algorithm" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::find_shortest_path_help() const noexcept{
std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - shortest_path <vertex-id> <vertex-id>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Finds the shortest path from one vertex to another in the graph" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::update_visualization_help() const noexcept{
std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - update_viz" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Draws a graph on request" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void GraphView::exit_help() const noexcept{ 
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - exit" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "End the program" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

}
}