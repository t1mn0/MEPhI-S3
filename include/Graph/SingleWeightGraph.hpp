#pragma once

#include <concepts>

#include "MultiWeightGraph.hpp"
#include "../Associative/HashSet.hpp"

namespace tmn {

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
class Graph<is_oriented, VertexId, VertexType, Weight> { 
private:
// Support structures :
    struct Edge;

public:
// Fields & aliases :
    using ConnectedVerticesList = HashTable<VertexId, Edge>;
    using TwoConnectedVertices = tmn::Pair<VertexId, VertexId>;
    using GraphPath = ListSequence<VertexId>;
    using IntMatrix = tmn::sequence::ArraySequence<tmn::sequence::ArraySequence<int>>;

    HashTable<VertexId, ConnectedVerticesList> adjacency_list;
    HashTable<VertexId, VertexType> resources;

public:
// Constructors & assignment & conversion & destructor:
    Graph() = default;
    Graph(const Graph& other) noexcept;
    Graph(Graph&& other) noexcept;
    Graph(const ArraySequence<tmn::Pair<TwoConnectedVertices, Weight>>& edges);

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

    bool add_edge(VertexId from, VertexId to, const Weight& weight, bool strict = true);
    bool add_edge(VertexId from, VertexId to, Weight&& weight, bool strict = true);
    bool remove_edge(VertexId from, VertexId to, bool strict = true);
    bool is_connected(VertexId from, VertexId to, bool strict = true) const;
    tmn::Optional<Weight> pass_weight(VertexId from, VertexId to) const noexcept;

    void reserve(std::size_t capacity); 
    void clear();

    tmn::Pair<IntMatrix, ArraySequence<VertexId>> basic_adjacency_list() const noexcept;
};

}

#include "SingleWeightGraphEdge.hpp"
#include "../../src/Graph/SingleWeightGraph.tpp"