#pragma once

#include "../Tuple/Tuple.hpp"
#include "../Associative/HashTable.hpp"
#include "../Sequence/ArraySequence.hpp"
#include "../Sequence/ListSequence.hpp"
#include "../SmartPtr/SharedPtr.hpp"

#include <cstdint> // uint8_t
#include <type_traits>
#include <concepts>

namespace tmn {
namespace graph {

using namespace tmn::sequence;
using namespace tmn::associative;

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
class Graph {
private:
// Support structures :
    struct Edge;

public:
// Fields & aliases :
    using ConnectedVerticesList = HashTable<VertexId, SharedPtr<Edge>>;
    using GraphPath = ListSequence<VertexId>;
    using WeightsTuple = Tuple<WeightTypes...>;

    HashTable<VertexId, ConnectedVerticesList> adjacency_list;
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

    void add_edge(VertexId from, VertexId to, WeightTypes... weights);
    void remove_edge(VertexId from, VertexId to);
    void change_edge_weights(VertexId from, VertexId to, const Tuple<WeightTypes...>& weights);
    tmn::Optional<WeightsTuple> bandwidth(VertexId from, VertexId to);

    tmn::Pair<ArraySequence<ArraySequence<uint8_t>>, ArraySequence<VertexId>> basic_adjacency_list();

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
}

#include "MultiWeightGraphEdge.hpp"
#include "SingleWeightGraph.hpp"
#include "UnweightedGraph.hpp"
#include "../../src/Graph/MultiWeightGraph.tpp"