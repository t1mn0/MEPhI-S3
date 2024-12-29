#include <utility>
#include <map>

#include "../../include/Graph/UnweightedGraph.hpp"

namespace tmn {
namespace graph {

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::Graph(const Graph<is_oriented, VertexId, VertexType, void>& other) noexcept :
    adjacency_list(other.adjacency_list), resources(other.resources) {}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::Graph(Graph<is_oriented, VertexId, VertexType, void>&& other) noexcept : 
    adjacency_list(std::move(other.adjacency_list)), resources(std::move(other.resources)) {
        other.adjacency_list.clear();
        other.resources.clear();
    }

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::Graph(const ArraySequence<TwoConnectedVertices>& edges) {
    for (const auto& edge : edges){
        if (!adjacency_list.contains(edge.first)) {
            adjacency_list.insert({edge.first, ConnectedVerticesList()});
        }
        adjacency_list[edge.first].insert(edge.second);

        if (!adjacency_list.contains(edge.second)) {
            adjacency_list.insert({edge.second, ConnectedVerticesList()});
        }

        if (!is_oriented){
            adjacency_list[edge.second].insert(edge.first);
        }
    }
}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::Graph(const ArraySequence< tmn::Pair< VertexId, ArraySequence<VertexId>> >& edges) {
    for (const auto& connected_vertices : edges){
        if (!adjacency_list.contains(connected_vertices.first)) {
            adjacency_list.insert({connected_vertices.first, ConnectedVerticesList()});
        }

        for (const auto& vertex_id : connected_vertices.second){
            adjacency_list[connected_vertices.first].insert(vertex_id);

            if (!adjacency_list.contains(vertex_id)) {
                adjacency_list.insert({vertex_id, ConnectedVerticesList()});
            }

            if (!is_oriented){
                adjacency_list[vertex_id].insert(connected_vertices.first);
            }
        }
    }
}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::Graph(const std::initializer_list<tmn::Pair<VertexId, VertexId>>& edges) {
    for (const auto& edge : edges){
        if (!adjacency_list.contains(edge.first)) {
            adjacency_list.insert({edge.first, ConnectedVerticesList()});
        }
        adjacency_list[edge.first].insert(edge.second);

        if (!adjacency_list.contains(edge.second)) {
            adjacency_list.insert({edge.second, ConnectedVerticesList()});
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
            adjacency_list.insert({connected_vertices.first, ConnectedVerticesList()});
        }

        for (const auto& vertex_id : connected_vertices.second){
            adjacency_list[connected_vertices.first].insert(vertex_id);

            if (!adjacency_list.contains(vertex_id)) {
                adjacency_list.insert({vertex_id, ConnectedVerticesList()});
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

        other_graph.adjacency_list.clear();
        other_graph.resources.clear();

        return *this;
}

template <bool is_oriented, typename VertexId, typename VertexType>
Graph<is_oriented, VertexId, VertexType, void>::~Graph() {
    adjacency_list.clear();
    resources.clear();
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool tmn::Graph<is_oriented, VertexId, VertexType, void>::oriented() const noexcept {
    return is_oriented;
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::add_vertex(VertexId vertex_id) noexcept {
    if (adjacency_list.contains(vertex_id)){
        return false;
    }

    adjacency_list.insert({vertex_id, ConnectedVerticesList()});
    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::add_vertex(VertexId vertex_id, const VertexType& vertex_resource, bool strict) {
    if (strict && adjacency_list.contains(vertex_id)){
        throw tmn::LogicException("Error(add_vertex) : graph already contains vertix with such VertexId: " + std::to_string(vertex_id));
    }
    else if (adjacency_list.contains(vertex_id)){
        return false;
    }

    adjacency_list.insert({vertex_id, ConnectedVerticesList()});
    resources.insert({vertex_id, vertex_resource});
    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::add_vertex(VertexId vertex_id, VertexType&& vertex_resource, bool strict) {
    if (strict && adjacency_list.contains(vertex_id)){
        throw tmn::LogicException("Error(add_vertex) : graph already contains vertix with such VertexId: " + std::to_string(vertex_id));
    }
    else if (adjacency_list.contains(vertex_id)){
        return false;
    }

    adjacency_list.insert({vertex_id, ConnectedVerticesList()});
    resources.insert({vertex_id, std::move(vertex_resource)});
    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::remove_vertex(VertexId vertex_id, bool strict) {
    if (strict && !adjacency_list.contains(vertex_id)){
        throw tmn::LogicException("Error(remove_vertex) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }
    else if (!adjacency_list.contains(vertex_id)){
        return false;
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
    return true;
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
tmn::Optional<std::size_t> Graph<is_oriented, VertexId, VertexType, void>::connected_vertices_count(VertexId vertex_id) const noexcept {
    if (!adjacency_list.contains(vertex_id)){
        return tmn::Optional<std::size_t>();
    }

    return tmn::Optional<std::size_t>(adjacency_list[vertex_id].size());
}

template <bool is_oriented, typename VertexId, typename VertexType>
HashSet<VertexId> Graph<is_oriented, VertexId, VertexType, void>::connected_vertices(VertexId vertex_id) const {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::LogicException("Error(connected_vertices) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }
    
    return adjacency_list[vertex_id];
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::change_vertex_id(VertexId old_vertex_id, VertexId new_vertex_id) {
    if (!adjacency_list.contains(old_vertex_id)){
        throw tmn::LogicException("Error(change_vertex_id) : vertex with such a VertexId is not in the graph: " + std::to_string(old_vertex_id));
    }

    if (adjacency_list.contains(new_vertex_id)){
        throw tmn::LogicException("Error(change_vertex_id) : graph already contains vertix with such VertexId: " + std::to_string(new_vertex_id));
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
        throw tmn::LogicException("Error(get_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    if (!resources.contains(vertex_id)){
        throw tmn::LogicException("Error(get_resource) : vertex with such a VertexId does not contain a resource: " + std::to_string(vertex_id));
    }

    return resources[vertex_id];
}

template <bool is_oriented, typename VertexId, typename VertexType>
VertexType& Graph<is_oriented, VertexId, VertexType, void>::get_resource(VertexId vertex_id) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::LogicException("Error(get_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    if (!resources.contains(vertex_id)){
        throw tmn::LogicException("Error(get_resource) : vertex with such a VertexId does not contain a resource: " + std::to_string(vertex_id));
    }

    return resources[vertex_id];
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::LogicException("Error(change_vertex_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    resources[vertex_id] = vertex_resource;
}

template <bool is_oriented, typename VertexId, typename VertexType>
void Graph<is_oriented, VertexId, VertexType, void>::change_vertex_resource(VertexId vertex_id, VertexType&& vertex_resource) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::LogicException("Error(change_vertex_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    resources[vertex_id] = std::move(vertex_resource);
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::add_edge(VertexId from, VertexId to, bool strict) {
    if (strict && !adjacency_list.contains(from)){
        throw tmn::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }
    else if (!adjacency_list.contains(from)) {
        return false;
    }

    if (strict && !adjacency_list.contains(to)){
        throw tmn::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }
    else if (!adjacency_list.contains(to)){
        return false;
    }

    adjacency_list[from].insert(to);

    if (!is_oriented){
        adjacency_list[to].insert(from);
    }

    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::remove_edge(VertexId from, VertexId to, bool strict) {
    if (strict && !adjacency_list.contains(from)){
        throw tmn::LogicException("Error(remove_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }
    else if (!adjacency_list.contains(from)){
        return false;
    }

    if (strict && !adjacency_list.contains(to)){
        throw tmn::LogicException("Error(remove_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }
    else if (!adjacency_list.contains(to)){
        return false;
    }

    if (strict && !adjacency_list[from].contains(to)){
        throw tmn::LogicException("Error(remove_edge) : vertices with the specified VertexId are not connected: " +  std::to_string(from) + " and " + std::to_string(to));
    }
    else if (!adjacency_list[from].contains(to)){
        return false;
    }

    adjacency_list[from].erase(to);

    if (!is_oriented){
        adjacency_list[to].erase(from);
    }
    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::is_connected(VertexId from, VertexId to, bool strict) const {
    if (strict && !adjacency_list.contains(from)){
        throw tmn::LogicException("Error(is_connected) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }
    else if (!adjacency_list.contains(from)){
        return false;
    }

    if (strict && !adjacency_list.contains(to)){
        throw tmn::LogicException("Error(is_connected) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }
    else if (!adjacency_list.contains(from)){
        return false;
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
    IntMatrix matrix(vertices.size(), ArraySequence<int>(vertices.size(), 0)); 

    tmn::HashTable<VertexId, size_t> vertex_index;
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertex_index[vertices[i]] = i;
    }

    for (const auto& vertex_pair : adjacency_list) {
        VertexId u = vertex_pair.first;
        const ConnectedVerticesList& connected_vertices = vertex_pair.second;
        size_t u_index = vertex_index[u];

        for (const VertexId& v : connected_vertices) {
            size_t v_index = vertex_index[v];
            matrix[u_index][v_index] = 1;
            if (!is_oriented) {
                matrix[v_index][u_index] = 1;
            }
        }
    }

    return tmn::Pair<IntMatrix, ArraySequence<VertexId>>(matrix, vertices);
}

}
}