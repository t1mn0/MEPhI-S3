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
    using ConnectedVerticesList = HashSet<VertexId>;
    using TwoConnectedVertices = tmn::Pair<VertexId, VertexId>;
    using GraphPath = ListSequence<VertexId>;
    using IntMatrix = tmn_sequence::ArraySequence<tmn_sequence::ArraySequence<int>>;

    HashTable<VertexId, ConnectedVerticesList> adjacency_list;
    HashTable<VertexId, VertexType> resources;

public:
// Constructors & assignment & conversion & destructor:
    Graph() = default;
    Graph(const Graph& other) noexcept;
    Graph(Graph&& other) noexcept;
    Graph(const ArraySequence<TwoConnectedVertices>& edges);                                                // graph without resources 
    Graph(const ArraySequence< tmn::Pair< VertexId, ArraySequence<VertexId>> >& edges);                     // graph without resources
    Graph(const std::initializer_list<tmn::Pair<VertexId, VertexId>>& list);                                // graph without resources
    Graph(const std::initializer_list< tmn::Pair<VertexId, std::initializer_list<VertexId>> >& list);       // graph without resources

    Graph& operator= (const Graph&);
    Graph& operator= (Graph&&);
    ~Graph();

// Basic methods:
    bool oriented() const noexcept;

    bool add_vertex(VertexId vertex_id) noexcept;
    bool add_vertex(VertexId vertex_id, const VertexType& vertex_resource, bool strict = true);
    bool add_vertex(VertexId vertex_id, VertexType&& vertex_resource, bool strict = true);
    bool remove_vertex(VertexId vertex_id, bool strict = true);
    bool vertex_in_graph(VertexId vertex_id) const noexcept;
    HashSet<VertexId> all_vertices() const noexcept;
    std::size_t v_size() const noexcept; 
    tmn::Optional<std::size_t> connected_vertices_count(VertexId vertex_id) const noexcept;
    HashSet<VertexId> connected_vertices(VertexId vertex_id) const;
    void change_vertex_id(VertexId old_vertex_id, VertexId new_vertex_id);

    const VertexType& get_resource(VertexId vertex_id) const;
    VertexType& get_resource(VertexId vertex_id);
    void change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource);
    void change_vertex_resource(VertexId vertex_id, VertexType&& vertex_resource);

    bool add_edge(VertexId from, VertexId to, bool strict = true);
    bool remove_edge(VertexId from, VertexId to, bool strict = true);
    bool is_connected(VertexId from, VertexId to, bool strict = true) const;

    void reserve(std::size_t capacity); 
    void clear();

    tmn::Pair<IntMatrix, ArraySequence<VertexId>> basic_adjacency_list() const noexcept;

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