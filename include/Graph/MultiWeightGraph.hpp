#pragma once

#include "../Tuple/Tuple.hpp"
#include "../Associative/HashTable.hpp"
#include "../Associative/HashSet.hpp"
#include "../Sequence/ArraySequence.hpp"
#include "../Sequence/ListSequence.hpp"
#include "../SmartPtr/SharedPtr.hpp"
#include "../SmartPtr/UniquePtr.hpp"
#include "../Function/Function.hpp"
#include "../tmn.hpp"

#include <functional>

namespace tmn {
namespace graph {

using namespace tmn::sequence;
using namespace tmn::associative;
using namespace tmn::smart_ptr;

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
class Graph {
private:
// Support structures :
    struct Edge;

public:
// Fields & aliases :
    using ConnectedVerticesList = HashTable<VertexId, Edge>;
    using TwoConnectedVertices = tmn::Pair<VertexId, VertexId>;
    using GraphPath = ListSequence<VertexId>;
    using WeightsTuple = Tuple<WeightTypes...>;
    using IntMatrix = ArraySequence<ArraySequence<int>>;

    Function<ResultWeight(WeightTypes...)> weight_calculation_function;

    HashTable<VertexId, ConnectedVerticesList> adjacency_list;
    HashTable<VertexId, VertexType> resources;

public:
// Constructors & assignment & conversion & destructor:
    Graph() = default;
    Graph(const Graph& other) noexcept;
    Graph(Graph&& other) noexcept;

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

    const VertexType& get_resource(VertexId vertex_id) const;
    VertexType& get_resource(VertexId vertex_id);
    void change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource);
    void change_vertex_resource(VertexId vertex_id, VertexType&& vertex_resource);

    bool change_weight_calculation_function(std::function<ResultWeight(WeightTypes...)> new_function) noexcept;

    void add_edge(VertexId from, VertexId to, const WeightTypes& ... weight_package);
    void add_edge(VertexId from, VertexId to, WeightTypes&& ... weight_package);
    bool remove_edge(VertexId from, VertexId to, bool strict = true);
    bool is_connected(VertexId from, VertexId to, bool strict = true) const;
    tmn::Optional<ResultWeight> calculate_pass_weight(VertexId from, VertexId to) const noexcept;

    void reserve(std::size_t capacity); 
    void clear();

    tmn::Pair<IntMatrix, ArraySequence<VertexId>> basic_adjacency_list() const noexcept;
};

}
}

#include "MultiWeightGraphEdge.hpp"
#include "SingleWeightGraph.hpp"
#include "UnweightedGraph.hpp"

#include "../../src/Graph/MultiWeightGraph.tpp"