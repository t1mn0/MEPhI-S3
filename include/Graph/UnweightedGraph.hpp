#pragma once

#include <initializer_list>
#include <cstdint>

#include "WeightedGraph.hpp"
#include "../Associative/HashSet.hpp"

namespace tmn {
namespace graph {

template <bool is_oriented, typename VertexId, typename VertexType, typename WeightType>
auto make_unweighted_graph(const Graph<is_oriented, VertexId, VertexType, WeightType>& weighted_graph)
    -> Graph<is_oriented, VertexId, VertexType, void>;


template <bool is_oriented, typename VertexId, typename VertexType>
class Graph<is_oriented, VertexId, VertexType, void> {
private:
// Fields & aliases :
    using ConnectedVerticesList = HashSet<VertexId>;
    using TwoConnectedVertices = tmn::Pair<VertexId, VertexId>;
    using GraphPath = ArraySequence<VertexId>;
    using IntMatrix = tmn::sequence::ArraySequence<tmn::sequence::ArraySequence<int>>;

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
    bool weighted() const noexcept;

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

    std::size_t posititve_vertex_degree(VertexId v, bool strict = false) const noexcept; 
    std::size_t negative_vertex_degree(VertexId v, bool strict = false) const noexcept;

    bool has_resource_at(VertexId vertex_id, bool strict = false) const;
    const VertexType& get_resource(VertexId vertex_id) const;
    VertexType& get_resource(VertexId vertex_id);
    void change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource);
    void change_vertex_resource(VertexId vertex_id, VertexType&& vertex_resource);

    bool add_edge(VertexId from, VertexId to, bool strict = true);
    bool remove_edge(VertexId from, VertexId to, bool strict = true);
    bool has_connected_vertices(VertexId from, VertexId to, bool strict = true) const;

    void add_connected_vertices(VertexId from, VertexId to);

    void reserve(std::size_t capacity); 
    void clear();

    tmn::Pair<IntMatrix, ArraySequence<VertexId>> basic_adjacency_list() const noexcept;

// Algorithms:
    bool has_cycle() const;
    bool is_connected_graph() const;

    GraphPath bfs() const;
    GraphPath bfs(VertexId start_vertex) const;
    GraphPath dfs() const;
    GraphPath dfs(VertexId start_vertex) const;

    HashTable<VertexId, int> graph_coloring() const; // ColorTable: VertedId-ColorId

    GraphPath find_shortest_path(VertexId start, VertexId end) const;

// Other:
    template<bool is_oriented_, typename VertexId_, typename VertexType_, typename WeightType_>
    friend Graph<is_oriented_, VertexId_, VertexType_, WeightType_> make_weighted_graph(const Graph<is_oriented_, VertexId_, VertexType_, void>& unweighted_graph, WeightType_ base_weight);
};

}
}

#include "../../src/Graph/UnweightedGraph.tpp"