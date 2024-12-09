#pragma once

#include <initializer_list>
#include <cstdint>

#include "SingleWeightGraph.hpp"
#include "../Associative/HashSet.hpp"

namespace tmn_graph {

template <bool is_oriented, typename VertexId, typename VertexType>
class Graph<is_oriented, VertexId, VertexType, void> {
private:
// Fields & aliases :
    using ConnectedVertices = HashSet<VertexId>;
    using GraphPath = ListSequence<VertexId>;

    HashTable<VertexId, ConnectedVertices> adjacency_list;
    HashTable<VertexId, VertexType> resources;

public:
// Constructors & assignment & conversion & destructor:
    Graph() = default;
    Graph(const Graph& other) noexcept;
    Graph(Graph&& other) noexcept;
    Graph(const ArraySequence<tmn::Pair<VertexId, VertexId>>& edges); // graph without resources
    Graph(const std::initializer_list<tmn::Pair<VertexId, VertexId>>& list); // graph without resources
    Graph(const std::initializer_list<tmn::Pair<VertexId, std::initializer_list<VertexId> >>& list); // graph without resources

    Graph& operator= (const Graph&);
    Graph& operator= (Graph&&);
    ~Graph();

// Basic methods:
    void add_vertex(VertexId vertex_id);
    void add_vertex(VertexId vertex_id, const VertexType& vertex_resource);
    void add_vertex(VertexId vertex_id, VertexType&& vertex_resource);
    void remove_vertex(VertexId vertex_id);
    bool vertex_in_graph(VertexId vertex_id) const noexcept;
    HashSet<VertexId> all_vertices() const noexcept;
    std::size_t v_size() const noexcept; 
    std::size_t connected_vertices_count(VertexId vertex_id) const;
    HashSet<VertexId> connected_vertices(VertexId vertex_id) const;
    void change_vertex_id(VertexId old_vertex_id, VertexId new_vertex_id);

    const VertexType& get_resource(VertexId vertex_id) const;
    VertexType& get_resource(VertexId vertex_id);
    void change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource);
    void change_vertex_resource(VertexId vertex_id, VertexType&& vertex_resource);

    void add_edge(VertexId from, VertexId to);                                       // TODO : TEST 
    void remove_edge(VertexId from, VertexId to);                                    // TODO : TEST 
    bool edge_in_graph(VertexId from, VertexId to) const;                            // TODO : TEST 

    void reserve(std::size_t capacity);                                              // TODO : TEST 
    void clear();                                                                    // TODO : TEST 

    tmn::Pair<ArraySequence<ArraySequence<int>>, ArraySequence<VertexId>> basic_adjacency_list() const noexcept;

// Algorithms:
    GraphPath dijkstra_shortest_path(VertexId from, VertexId to) const;

    GraphPath bfs() const;
    GraphPath dfs() const;

    Graph connected_component() const;
    Graph strongly_connected_component() const;

    Graph prim_algorithm() const; // Spanning tree search
    Graph kruskal_algorithm() const; // Spanning tree search
};

}

#include "../../src/Graph/UnweightedGraph.tpp"