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
protected:
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
    virtual bool oriented() const noexcept;
    virtual bool weighted() const noexcept;

    virtual bool add_vertex(VertexId vertex_id) noexcept;
    virtual bool add_vertex(VertexId vertex_id, const VertexType& vertex_resource, bool strict = true);
    virtual bool add_vertex(VertexId vertex_id, VertexType&& vertex_resource, bool strict = true);
    virtual bool remove_vertex(VertexId vertex_id, bool strict = true);
    virtual bool vertex_in_graph(VertexId vertex_id) const noexcept;
    virtual HashSet<VertexId> all_vertices() const noexcept;
    virtual std::size_t v_size() const noexcept; 
    virtual tmn::Optional<std::size_t> connected_vertices_count(VertexId vertex_id) const noexcept;
    virtual HashSet<VertexId> connected_vertices(VertexId vertex_id) const;
    virtual void change_vertex_id(VertexId old_vertex_id, VertexId new_vertex_id);

    virtual std::size_t posititve_vertex_degree(VertexId v, bool strict = false) const noexcept; 
    virtual std::size_t negative_vertex_degree(VertexId v, bool strict = false) const noexcept;

    virtual bool has_resource_at(VertexId vertex_id, bool strict = false) const;
    virtual const VertexType& get_resource(VertexId vertex_id) const;
    virtual VertexType& get_resource(VertexId vertex_id);
    virtual void change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource);
    virtual void change_vertex_resource(VertexId vertex_id, VertexType&& vertex_resource);

    virtual bool add_edge(VertexId from, VertexId to, bool strict = true);
    virtual bool remove_edge(VertexId from, VertexId to, bool strict = true);
    virtual bool has_connected_vertices(VertexId from, VertexId to, bool strict = true) const;

    virtual void add_connected_vertices(VertexId from, VertexId to);

    virtual void reserve(std::size_t capacity); 
    virtual void clear();

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