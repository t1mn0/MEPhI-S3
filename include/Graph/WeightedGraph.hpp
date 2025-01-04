#pragma once

#include "../Associative/HashTable.hpp"
#include "../Associative/HashSet.hpp"
#include "../Sequence/ArraySequence.hpp"
#include "../Sequence/ListSequence.hpp"
#include "../Adapter/Queue.hpp"
#include "../Adapter/Stack.hpp"
#include "../Function/Function.hpp"
#include "../tmn.hpp"

namespace tmn {
namespace graph {

using namespace tmn::sequence;
using namespace tmn::associative;
using namespace tmn::adapter;

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
class Graph;

template <bool is_oriented, typename VertexId, typename VertexType, typename WeightType>
auto make_weighted_graph(const Graph<is_oriented, VertexId, VertexType, void>& unweighted_graph, WeightType base_weight = WeightType())
    -> Graph<is_oriented, VertexId, VertexType, WeightType>;



template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
class Graph { 
private:
// Support structures :
    struct Edge;

public:
// Fields & aliases :
    using ConnectedVerticesList = HashTable<VertexId, Edge>;
    using TwoConnectedVertices = tmn::Pair<VertexId, VertexId>;
    using GraphPath = ArraySequence<VertexId>;
    using IntMatrix = ArraySequence<ArraySequence<int>>;
    using WeightMatrix = ArraySequence<ArraySequence<Weight>>;

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

    std::size_t postitve_vertex_degree(VertexId v, bool strict = false) const noexcept; 
    std::size_t negative_vertex_degree(VertexId v, bool strict = false) const noexcept;

    bool has_resource_at(VertexId vertex_id, bool strict = false) const;
    const VertexType& get_resource(VertexId vertex_id) const;
    VertexType& get_resource(VertexId vertex_id);
    void change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource);
    void change_vertex_resource(VertexId vertex_id, VertexType&& vertex_resource);

    bool add_edge(VertexId from, VertexId to, bool strict = true);
    bool add_edge(VertexId from, VertexId to, const Weight& weight, bool strict = true);
    bool add_edge(VertexId from, VertexId to, Weight&& weight, bool strict = true);
    bool remove_edge(VertexId from, VertexId to, bool strict = true);
    bool is_connected(VertexId from, VertexId to, bool strict = true) const;
    tmn::Optional<Weight> pass_weight(VertexId from, VertexId to) const noexcept;

    void add_connected_vertices(VertexId from, VertexId to, const Weight& weight);

    void reserve(std::size_t capacity); 
    void clear();

    tmn::Pair<IntMatrix, ArraySequence<VertexId>> basic_adjacency_list() const noexcept;
    tmn::Pair<WeightMatrix, ArraySequence<VertexId>> weighted_adjacency_matrix() const noexcept;

// Algorithms:
    bool has_cycle() const;
    bool is_connected_graph() const;

    GraphPath bfs() const;
    GraphPath bfs(VertexId start_vertex) const;
    GraphPath dfs() const;
    GraphPath dfs(VertexId start_vertex) const;

    HashTable<VertexId, int> graph_coloring() const; // ColorTable: VertedId-ColorId


// Other:
    template<bool is_oriented_, typename VertexId_, typename VertexType_, typename WeightType_>
    friend Graph<is_oriented_, VertexId_, VertexType_, void> make_unweighted_graph(const Graph<is_oriented_, VertexId_, VertexType_, WeightType_>& weighted_graph);   
};

}
}

#include "WeightedGraphEdge.hpp"
#include "UnweightedGraph.hpp"
#include "../../src/Graph/WeightedGraph.tpp"
#include "../../src/Graph/MakeGraph/Conversion.tpp"