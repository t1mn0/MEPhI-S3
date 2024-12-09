#include <utility>

#include "../../include/Graph/UnweightedGraph.hpp"

namespace tmn_graph {

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::Graph(const Graph<is_oriented, VertexId, VertexType, void>& other) noexcept :
    adjacency_list(other.adjacency_list), resources(other.resources) {}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::Graph(Graph<is_oriented, VertexId, VertexType, void>&& other) noexcept : 
    adjacency_list(std::move(other.adjacency_list)), resources(std::move(other.resources)) {}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::Graph(const ArraySequence<tmn::Pair<VertexId, VertexId>>& edges) {
    for (const auto& edge : edges){
        if (!adjacency_list.contains(edge.first)) {
            adjacency_list.insert({edge.first, ConnectedVertices()});
        }
        adjacency_list[edge.first].insert(edge.second);

        if (!adjacency_list.contains(edge.second)) {
            adjacency_list.insert({edge.second, ConnectedVertices()});
        }

        if (!is_oriented){
            adjacency_list[edge.second].insert(edge.first);
        }
    }
}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::Graph(const std::initializer_list<tmn::Pair<VertexId, VertexId>>& edges) {
    for (const auto& edge : edges){
        if (!adjacency_list.contains(edge.first)) {
            adjacency_list.insert({edge.first, ConnectedVertices()});
        }
        adjacency_list[edge.first].insert(edge.second);

        if (!adjacency_list.contains(edge.second)) {
            adjacency_list.insert({edge.second, ConnectedVertices()});
        }

        if (!is_oriented){
            adjacency_list[edge.second].insert(edge.first);
        }
    }
}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::Graph(const std::initializer_list<tmn::Pair<VertexId, std::initializer_list<VertexId> >>& list) {
    for (const auto& connected_vertices : list){
        if (!adjacency_list.contains(connected_vertices.first)) {
            adjacency_list.insert({connected_vertices.first, ConnectedVertices()});
        }

        for (const auto& vertex_id : connected_vertices.second){
            adjacency_list[connected_vertices.first].insert(vertex_id);

            if (!adjacency_list.contains(vertex_id)) {
                adjacency_list.insert({vertex_id, ConnectedVertices()});
            }

            if (!is_oriented){
                adjacency_list[vertex_id].insert(connected_vertices.first);
            }
        }
    }
}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>& 
    Graph<is_oriented, VertexId, VertexType, void>::operator=(const Graph<is_oriented, VertexId, VertexType, void>& other_graph) {

        adjacency_list = other_graph.adjacency_list;
        resources = other_graph.resources;

        return *this;
}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>& 
    Graph<is_oriented, VertexId, VertexType, void>::operator=(Graph<is_oriented, VertexId, VertexType, void>&& other_graph) {

        adjacency_list = std::move(other_graph.adjacency_list);
        resources = std::move(other_graph.resources);

        return *this;
}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::~Graph() {
    adjacency_list.clear();
    resources.clear();
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::add_vertex(VertexId vertex_id) {
    if (adjacency_list.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(add_vertex) : graph already contains vertix with such VertexId: " + std::to_string(vertex_id));
    }

    adjacency_list.insert({vertex_id, ConnectedVertices()});
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::add_vertex(VertexId vertex_id, const VertexType& vertex_resource) {
    if (adjacency_list.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(add_vertex) : graph already contains vertix with such VertexId: " + std::to_string(vertex_id));
    }

    adjacency_list.insert({vertex_id, ConnectedVertices()});
    resources.insert({vertex_id, vertex_resource});
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::add_vertex(VertexId vertex_id, VertexType&& vertex_resource) {
    if (adjacency_list.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(add_vertex) : graph already contains vertix with such VertexId: " + std::to_string(vertex_id));
    }

    adjacency_list.insert({vertex_id, ConnectedVertices()});
    resources.insert({vertex_id, std::move(vertex_resource)});
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::remove_vertex(VertexId vertex_id) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(remove_vertex) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    for (auto& connected_vertex_id : adjacency_list[vertex_id]) {
        adjacency_list[connected_vertex_id].erase(vertex_id);
    }

    if (is_oriented){
        for (auto& connected_vertex_id : adjacency_list){
            if (connected_vertex_id.second.contains(vertex_id)){
                connected_vertex_id.second.erase(vertex_id);
            }
        }
    }

    adjacency_list.erase(vertex_id);

    if(resources.contains(vertex_id)){
        resources.erase(vertex_id);
    }
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::vertex_in_graph(VertexId vertex_id) const noexcept {
    return adjacency_list.contains(vertex_id);
}

template <bool is_oriented, typename VertexId, typename VertexType>
HashSet<VertexId> Graph<is_oriented, VertexId, VertexType, void>::all_vertices() const noexcept {
    HashSet<VertexId> result;
    for (auto& v : adjacency_list){
        result.insert(v.first);
    }
    return result;
}

template <bool is_oriented, typename VertexId, typename VertexType>
std::size_t Graph<is_oriented, VertexId, VertexType, void>::v_size() const noexcept {
    return adjacency_list.size();
}

template <bool is_oriented, typename VertexId, typename VertexType>
std::size_t Graph<is_oriented, VertexId, VertexType, void>::connected_vertices_count(VertexId vertex_id) const {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(connected_vertices_count) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }
    return adjacency_list[vertex_id].size();
}

template <bool is_oriented, typename VertexId, typename VertexType>
HashSet<VertexId> Graph<is_oriented, VertexId, VertexType, void>::connected_vertices(VertexId vertex_id) const {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(connected_vertices) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }
    return adjacency_list[vertex_id];
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::change_vertex_id(VertexId old_vertex_id, VertexId new_vertex_id) {
    if (!adjacency_list.contains(old_vertex_id)){
        throw tmn_exception::LogicException("Error(change_vertex_id) : vertex with such a VertexId is not in the graph: " + std::to_string(old_vertex_id));
    }

    if (adjacency_list.contains(new_vertex_id)){
        throw tmn_exception::LogicException("Error(change_vertex_id) : graph already contains vertix with such VertexId: " + std::to_string(new_vertex_id));
    }

    for (auto& connected_vertex_id : adjacency_list[old_vertex_id]) {
        adjacency_list[connected_vertex_id].erase(old_vertex_id);
        adjacency_list[connected_vertex_id].insert(new_vertex_id);
    }

    adjacency_list.insert({new_vertex_id, std::move(adjacency_list[old_vertex_id])});
    adjacency_list.erase(old_vertex_id);

    resources.insert({new_vertex_id, std::move(resources[old_vertex_id])});
    resources.erase(old_vertex_id);
}

template <bool is_oriented, typename VertexId, typename VertexType>
const VertexType& Graph<is_oriented, VertexId, VertexType, void>::get_resource(VertexId vertex_id) const {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(get_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    if (!resources.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(get_resource) : vertex with such a VertexId does not contain a resource: " + std::to_string(vertex_id));
    }

    return resources[vertex_id];
}

template <bool is_oriented, typename VertexId, typename VertexType>
VertexType& Graph<is_oriented, VertexId, VertexType, void>::get_resource(VertexId vertex_id) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(get_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    if (!resources.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(get_resource) : vertex with such a VertexId does not contain a resource: " + std::to_string(vertex_id));
    }

    return resources[vertex_id];
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(change_vertex_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    resources[vertex_id] = vertex_resource;
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::change_vertex_resource(VertexId vertex_id, VertexType&& vertex_resource) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn_exception::LogicException("Error(change_vertex_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    resources[vertex_id] = std::move(vertex_resource);
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::add_edge(VertexId from, VertexId to) {
    if (!adjacency_list.contains(from)){
        throw tmn_exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }

    if (!adjacency_list.contains(to)){
        throw tmn_exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }

    adjacency_list[from].insert(to);

    if (!is_oriented){
        adjacency_list[to].insert(from);
    }
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::remove_edge(VertexId from, VertexId to) {
    if (!adjacency_list.contains(from)){
        throw tmn_exception::LogicException("Error(remove_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }

    if (!adjacency_list.contains(to)){
        throw tmn_exception::LogicException("Error(remove_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }

    if (!adjacency_list[from].contains(to)){
        throw tmn_exception::LogicException("Error(remove_edge) : vertices with the specified VertexId are not connected: " +  std::to_string(from) + " and " + std::to_string(to));
    }

    adjacency_list[from].erase(to);

    if (!is_oriented){
        adjacency_list[to].erase(from);
    }
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::edge_in_graph(VertexId from, VertexId to) const {
    if (!adjacency_list.contains(from)){
        throw tmn_exception::LogicException("Error(edge_in_graph) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }

    if (!adjacency_list.contains(to)){
        throw tmn_exception::LogicException("Error(edge_in_graph) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }

    return adjacency_list[from].contains(to);
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::reserve(std::size_t capacity) {
    resources.reserve(capacity);
    adjacency_list.reserve(capacity);
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::clear() {
    adjacency_list.clear();
    resources.clear();
}

template <bool is_oriented, typename VertexId, typename VertexType>
tmn::Pair<ArraySequence<ArraySequence<int>>, ArraySequence<VertexId>> 
    Graph<is_oriented, VertexId, VertexType, void>::basic_adjacency_list() const noexcept {
        
        auto vertices = adjacency_list.keys();

        ArraySequence<ArraySequence<int>> matrix(vertices.size());
    
        std::size_t i = 0;
        for (auto& vertix_id : vertices) {
            std::size_t j = 0;
            for (auto& p : adjacency_list) {
                if (p.second.contains(vertix_id)){
                    matrix[i].push_back(1);
                }
                else{
                    matrix[i].push_back(0);
                }
                ++j;
            } 
            ++i;
        } 

    return tmn::Pair<ArraySequence<ArraySequence<int>>, ArraySequence<VertexId>>(matrix, vertices);
}

}