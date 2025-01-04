#include "../../../include/Graph/Graph.hpp"

namespace tmn {
namespace graph {

template <bool is_oriented, typename VertexId, typename VertexType>
auto make_complete_graph(size_t count_vertices, VertexId start_id)
    -> Graph<is_oriented, VertexId, VertexType, void> {

    Graph<is_oriented, VertexId, VertexType, void> graph;
    VertexId current_id = start_id;
    for (size_t i = 0; i < count_vertices; ++i){
        graph.add_vertex(current_id);
        ++current_id;
    }
    current_id = start_id;
    for (size_t i = 0; i < count_vertices; ++i){
        VertexId iter_id = start_id;
        for (size_t j = 0; j < count_vertices; ++j){
            if (iter_id != current_id) {
                graph.add_connected_vertices(current_id, iter_id);
                if (is_oriented){
                    graph.add_connected_vertices(iter_id, current_id);
                }
                ++iter_id;
            }
        }

        ++current_id;
    }
    
    return graph;
}

template <bool is_oriented, typename VertexId, typename VertexType>
auto make_bipartite_complete_graph(size_t count_vertices1, size_t count_vertices2, VertexId start_id1, VertexId start_id2)
    -> Graph<is_oriented, VertexId, VertexType, void> {

    if (start_id1 + count_vertices1 > start_id2){
        throw tmn::exception::LogicException("Error(make_bipartite_graph) : bad division into parts");
    }
    
    Graph<is_oriented, VertexId, VertexType, void> graph;

    VertexId current_id1 = start_id1;
    for (size_t i = 0; i < count_vertices1; ++i) {
        graph.add_vertex(current_id1);
        ++current_id1;
    }

    VertexId current_id2 = start_id2;
    for (size_t i = 0; i < count_vertices2; ++i) {
         graph.add_vertex(current_id2);
         ++current_id2;
    }

    current_id1 = start_id1;
    for (size_t i = 0; i < count_vertices1; ++i) {
        current_id2 = start_id2;
        for (size_t j = 0; j < count_vertices2; ++j) {
            graph.add_edge(current_id1, current_id2);
            
            if (!is_oriented){
                 graph.add_edge(current_id2, current_id1);
            }
             
            ++current_id2;
         }
         ++current_id1;
    }

    return graph;
}

template <bool is_oriented, typename VertexId, typename VertexType>
auto make_complement_graph(const Graph<is_oriented, VertexId, VertexType, void>& original_graph) 
    -> Graph<is_oriented, VertexId, VertexType, void> {

    Graph<is_oriented, VertexId, VertexType, void> complement_graph;

    HashSet<VertexId> vertices = original_graph.all_vertices();
    for(const auto& vertex : vertices) {
        complement_graph.add_vertex(vertex);
    }

    for (const auto& vertex1 : vertices) {
        for(const auto& vertex2 : vertices){
            if(vertex1 != vertex2) {
                if (!original_graph.is_connected(vertex1, vertex2)) {
                    complement_graph.add_edge(vertex1, vertex2);
                    if (!is_oriented){
                        complement_graph.add_edge(vertex2, vertex1);
                    }
                }
            }
        }
    }

    return complement_graph;
}

template <bool is_oriented, typename VertexId, typename VertexType>
auto make_simple_chain_graph(size_t count_vertices, VertexId start_id)
    -> Graph<is_oriented, VertexId, VertexType, void> {
        
    Graph<is_oriented, VertexId, VertexType, void> chain_graph;

    if(count_vertices == 0){
        return chain_graph;
    }

    VertexId current_id = start_id;
    for (size_t i = 0; i < count_vertices; ++i) {
        chain_graph.add_vertex(current_id);
        ++current_id;
    }

    current_id = start_id;
    for (size_t i = 0; i < count_vertices - 1; ++i) {
        chain_graph.add_edge(current_id, current_id + 1);
        if (!is_oriented){
            chain_graph.add_edge(current_id + 1, current_id);
        }
        ++current_id;
    }

    return chain_graph;
}

template <bool is_oriented, typename VertexId, typename VertexType>
auto make_wheel_graph(size_t count_vertices, VertexId center_id, VertexId start_cycle_id)
    -> Graph<is_oriented, VertexId, VertexType, void> {

    Graph<is_oriented, VertexId, VertexType, void> wheel_graph;

    if(count_vertices == 0) {
        return wheel_graph;
    }

    wheel_graph.add_vertex(center_id);

    VertexId current_cycle_id = start_cycle_id;
    for (size_t i = 0; i < count_vertices; ++i) {
        wheel_graph.add_vertex(current_cycle_id);
        ++current_cycle_id;
    }

    current_cycle_id = start_cycle_id;
    for(size_t i = 0; i < count_vertices; ++i) {
        wheel_graph.add_edge(center_id, current_cycle_id);

        if(!is_oriented){
            wheel_graph.add_edge(current_cycle_id, center_id);
        }
        
        ++current_cycle_id;
    }

    current_cycle_id = start_cycle_id;
    for (size_t i = 0; i < count_vertices - 1; ++i) {
        wheel_graph.add_edge(current_cycle_id, current_cycle_id + 1);
        
        if (!is_oriented){
            wheel_graph.add_edge(current_cycle_id + 1, current_cycle_id);
        }
        
        ++current_cycle_id;
    }

    if (count_vertices > 1){
        wheel_graph.add_edge(current_cycle_id, start_cycle_id);
        if(!is_oriented){
            wheel_graph.add_edge(start_cycle_id, current_cycle_id);
        }
    }

    return wheel_graph;
}

template <bool is_oriented, typename VertexId, typename VertexType>
auto make_biconnected_graph(size_t count_vertices, VertexId start_id)
    -> Graph<is_oriented, VertexId, VertexType, void>  {

    Graph<is_oriented, VertexId, VertexType, void> biconnected_graph;

    if(count_vertices <= 2) {
        return biconnected_graph;
    }

    VertexId current_id = start_id;
    for(size_t i = 0; i < count_vertices; ++i) {
        biconnected_graph.add_vertex(current_id);
        ++current_id;
    }

    current_id = start_id;
    for(size_t i = 0; i < count_vertices; ++i) {
        biconnected_graph.add_edge(current_id, current_id + 1 >= start_id + count_vertices ? start_id : current_id + 1);
        if(!is_oriented){
            biconnected_graph.add_edge(current_id + 1 >= start_id + count_vertices ? start_id : current_id + 1, current_id);
        }
        ++current_id;
    }

    for (size_t i = 0; i < count_vertices; ++i) {
        VertexId v1 = start_id + i;
        VertexId v2 = start_id + (i + 2) % count_vertices;
        biconnected_graph.add_edge(v1, v2);
        if (!is_oriented) {
            biconnected_graph.add_edge(v2, v1);
        }
    }

    return biconnected_graph;
}



}
}