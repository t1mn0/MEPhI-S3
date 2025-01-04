#pragma once
#include "../../../include/Graph/Graph.hpp"

namespace tmn {
namespace graph {
    
template <bool is_oriented, typename VertexId, typename VertexType, typename WeightType>
auto make_weighted_graph(const Graph<is_oriented, VertexId, VertexType, void>& unweighted_graph, WeightType base_weight)
    -> Graph<is_oriented, VertexId, VertexType, WeightType> {

    Graph<is_oriented, VertexId, VertexType, WeightType> weighted_graph;

    HashSet<VertexId> vertices = unweighted_graph.all_vertices();

    for(const auto& vertex : vertices) {
        weighted_graph.add_vertex(vertex);
    }

    for (const auto& vertex : vertices) {
        if(unweighted_graph.adjacency_list.contains(vertex)) {
            for (const auto& v : unweighted_graph.adjacency_list[vertex]) {
                weighted_graph.add_edge(vertex, v, base_weight);
            }
        }
    }

    return weighted_graph;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename WeightType>
auto make_unweighted_graph(const Graph<is_oriented, VertexId, VertexType, WeightType>& weighted_graph)
    -> Graph<is_oriented, VertexId, VertexType, void> {

    Graph<is_oriented, VertexId, VertexType, void> unweighted_graph;

    HashSet<VertexId> vertices = weighted_graph.all_vertices();

    for(const auto& vertex : vertices) {
        unweighted_graph.add_vertex(vertex);
    }

    for (const auto& vertex : vertices) {
        if(weighted_graph.adjacency_list.contains(vertex)) {
            for (const auto& edge_pair : weighted_graph.adjacency_list[vertex]) {
                unweighted_graph.add_edge(vertex, edge_pair.first);
            }
        }
    }

    return unweighted_graph;
}

}
}