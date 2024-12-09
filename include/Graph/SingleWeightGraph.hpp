#pragma once

#include "MultiWeightGraph.hpp"

namespace tmn_graph {

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
class Graph<is_oriented, VertexId, VertexType, Weight> { 
private:
// Support structures :
    class Edge;

public:
// Fields & aliases :
    using ConnectedVertices = HashTable<VertexId, SharedPtr<Edge>>;
    using GraphPath = ListSequence<VertexId>;

    HashTable<VertexId, ConnectedVertices> adjacency_list;
    HashTable<VertexId, VertexType> resources;

public:
// Constructors & assignment & conversion & destructor:
    Graph();
    Graph(const Graph& other);
    Graph(Graph&& other);
    Graph& operator= (const Graph&);
    Graph& operator= (Graph&&);
    ~Graph();

// Basic methods:
    void add_vertex(VertexId vertex_id, const VertexType& vertex_resource);
    void remove_vertex(VertexId vertex_id);
    void change_vertex_id(VertexId old_vertex_id, VertexId new_vertex_id);
    void change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource);

    void add_edge(VertexId from, VertexId to, Weight weight);
    void remove_edge(VertexId from, VertexId to);
    void change_edge_weights(VertexId from, VertexId to, Weight weight);
    tmn::Optional<Weight> bandwidth(VertexId from, VertexId to);

    tmn::Pair<ArraySequence<ArraySequence<uint8_t>>, ArraySequence<VertexId>> basic_adjacency_list();
    tmn::Pair<ArraySequence<ArraySequence<Weight>>, ArraySequence<VertexId>> detailed_adjacency_list();

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

#include "SingleWeightGraphEdge.hpp"
#include "../../src/Graph/SingleWeightGraph.tpp"