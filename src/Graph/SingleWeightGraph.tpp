#include <utility>

#include "../../include/Graph/SingleWeightGraph.hpp"

namespace tmn {
namespace graph {

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
Graph<is_oriented, VertexId, VertexType, Weight>::Graph(const Graph<is_oriented, VertexId, VertexType, Weight>& other) noexcept :
    adjacency_list(other.adjacency_list), resources(other.resources) {}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
Graph<is_oriented, VertexId, VertexType, Weight>::Graph(Graph<is_oriented, VertexId, VertexType, Weight>&& other) noexcept : 
    adjacency_list(std::move(other.adjacency_list)), resources(std::move(other.resources)) {
        other.adjacency_list.clear();
        other.resources.clear();
    }

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
Graph<is_oriented, VertexId, VertexType, Weight>::Graph(const ArraySequence<tmn::Pair<TwoConnectedVertices, Weight>>& edges) {
    for (const auto& edge : edges){
        if (!adjacency_list.contains(edge.first.first)) {
            adjacency_list.insert({edge.first.first, ConnectedVerticesList()});
        }

        if (!adjacency_list.contains(edge.first.second)) {
            adjacency_list.insert({edge.first.second, ConnectedVerticesList()});
        }

        Edge new_direct_edge(edge.first.first, edge.first.second, edge.second);

        adjacency_list[edge.first.first].insert({edge.first.second, new_direct_edge});

        if (!is_oriented){
            Edge new_back_edge(edge.first.second, edge.first.first, edge.second);
            adjacency_list[edge.first.second].insert({edge.first.first, new_back_edge});
        }
    }
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
Graph<is_oriented, VertexId, VertexType, Weight>& 
    Graph<is_oriented, VertexId, VertexType, Weight>::operator=(const Graph<is_oriented, VertexId, VertexType, Weight>& other_graph) {

        adjacency_list = other_graph.adjacency_list;
        resources = other_graph.resources;

        return *this;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
Graph<is_oriented, VertexId, VertexType, Weight>& 
    Graph<is_oriented, VertexId, VertexType, Weight>::operator=(Graph<is_oriented, VertexId, VertexType, Weight>&& other_graph) {

        adjacency_list = std::move(other_graph.adjacency_list);
        resources = std::move(other_graph.resources);

        other_graph.adjacency_list.clear();
        other_graph.resources.clear();

        return *this;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
Graph<is_oriented, VertexId, VertexType, Weight>::~Graph() {
    adjacency_list.clear();
    resources.clear();
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::oriented() const noexcept {
    return is_oriented;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::weighted() const noexcept {
    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::add_vertex(VertexId vertex_id) noexcept {
    if (adjacency_list.contains(vertex_id)){
        return false;
    }

    adjacency_list.insert({vertex_id, ConnectedVerticesList()});
    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::add_vertex(VertexId vertex_id, const VertexType& vertex_resource, bool strict) {
    if (strict && adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(add_vertex) : graph already contains vertix with such VertexId: " + std::to_string(vertex_id));
    }
    else if (adjacency_list.contains(vertex_id)){
        return false;
    }

    adjacency_list.insert({vertex_id, ConnectedVerticesList()});
    resources.insert({vertex_id, vertex_resource});
    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::add_vertex(VertexId vertex_id, VertexType&& vertex_resource, bool strict) {
    if (strict && adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(add_vertex) : graph already contains vertix with such VertexId: " + std::to_string(vertex_id));
    }
    else if (adjacency_list.contains(vertex_id)){
        return false;
    }

    adjacency_list.insert({vertex_id, ConnectedVerticesList()});
    resources.insert({vertex_id, std::move(vertex_resource)});
    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::remove_vertex(VertexId vertex_id, bool strict) {
    if (strict && !adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(remove_vertex) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }
    else if (!adjacency_list.contains(vertex_id)){
        return false;
    }

    for (auto& connected_vertex_id : adjacency_list[vertex_id]) {
        adjacency_list[connected_vertex_id.first].erase(vertex_id);
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

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::vertex_in_graph(VertexId vertex_id) const noexcept {
    return adjacency_list.contains(vertex_id);
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
HashSet<VertexId> Graph<is_oriented, VertexId, VertexType, Weight>::all_vertices() const noexcept {
    HashSet<VertexId> result;
    for (auto& v : adjacency_list){
        result.insert(v.first);
    }
    return result;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
std::size_t Graph<is_oriented, VertexId, VertexType, Weight>::v_size() const noexcept {
    return adjacency_list.size();
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
tmn::Optional<std::size_t> Graph<is_oriented, VertexId, VertexType, Weight>::connected_vertices_count(VertexId vertex_id) const noexcept {
    if (!adjacency_list.contains(vertex_id)){
        return tmn::Optional<std::size_t>();
    }
    return tmn::Optional<std::size_t>(adjacency_list[vertex_id].size());
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
HashSet<VertexId> Graph<is_oriented, VertexId, VertexType, Weight>::connected_vertices(VertexId vertex_id) const {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(connected_vertices) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }
    HashSet<VertexId> vertices;
    for (const auto& vertex_id : adjacency_list[vertex_id]){
        vertices.insert(vertex_id.first);
    }
    return vertices;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
const VertexType& Graph<is_oriented, VertexId, VertexType, Weight>::get_resource(VertexId vertex_id) const {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(get_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    if (!resources.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(get_resource) : vertex with such a VertexId does not contain a resource: " + std::to_string(vertex_id));
    }

    return resources[vertex_id];
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
VertexType& Graph<is_oriented, VertexId, VertexType, Weight>::get_resource(VertexId vertex_id) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(get_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    if (!resources.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(get_resource) : vertex with such a VertexId does not contain a resource: " + std::to_string(vertex_id));
    }

    return resources[vertex_id];
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
void Graph<is_oriented, VertexId, VertexType, Weight>::change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(change_vertex_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    resources[vertex_id] = vertex_resource;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
void Graph<is_oriented, VertexId, VertexType, Weight>::change_vertex_resource(VertexId vertex_id, VertexType&& vertex_resource) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(change_vertex_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    resources[vertex_id] = std::move(vertex_resource);
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::add_edge(VertexId from, VertexId to, bool strict){
    if (strict && !adjacency_list.contains(from)){
        throw tmn::exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }
    else if (!adjacency_list.contains(from)) {
        return false;
    }

    if (strict && !adjacency_list.contains(to)){
        throw tmn::exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }
    else if (!adjacency_list.contains(to)){
        return false;
    }

    Edge new_direct_edge(from, to, Weight{});

    adjacency_list[from].insert({to, new_direct_edge});

    if (!is_oriented){
        Edge new_back_edge(to, from, Weight{});
        adjacency_list[to].insert({from, new_back_edge});
    }

    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::add_edge(VertexId from, VertexId to, const Weight& weight, bool strict){
    if (strict && !adjacency_list.contains(from)){
        throw tmn::exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }
    else if (!adjacency_list.contains(from)) {
        return false;
    }

    if (strict && !adjacency_list.contains(to)){
        throw tmn::exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }
    else if (!adjacency_list.contains(to)){
        return false;
    }

    Edge new_direct_edge(from, to, weight);

    adjacency_list[from].insert({to, new_direct_edge});

    if (!is_oriented){
        Edge new_back_edge(to, from, weight);
        adjacency_list[to].insert({from, new_back_edge});
    }

    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::add_edge(VertexId from, VertexId to, Weight&& weight, bool strict) {
    if (strict && !adjacency_list.contains(from)){
        throw tmn::exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }
    else if (!adjacency_list.contains(from)) {
        return false;
    }

    if (strict && !adjacency_list.contains(to)){
        throw tmn::exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }
    else if (!adjacency_list.contains(to)){
        return false;
    }

    Edge new_direct_edge(from, to, std::move(weight));

    adjacency_list[from].insert({to, new_direct_edge});

    if (!is_oriented){
        Edge new_back_edge(to, from, new_direct_edge.weight);
        adjacency_list[to].insert({from, new_back_edge});
    }

    return true;
}


template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::remove_edge(VertexId from, VertexId to, bool strict) {
    if (strict && !adjacency_list.contains(from)){
        throw tmn::exception::LogicException("Error(remove_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }
    else if (!adjacency_list.contains(from)){
        return false;
    }

    if (strict && !adjacency_list.contains(to)){
        throw tmn::exception::LogicException("Error(remove_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }
    else if (!adjacency_list.contains(to)){
        return false;
    }

    if (strict && !adjacency_list[from].contains(to)){
        throw tmn::exception::LogicException("Error(remove_edge) : vertices with the specified VertexId are not connected: " +  std::to_string(from) + " and " + std::to_string(to));
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

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::is_connected(VertexId from, VertexId to, bool strict) const {
    if (strict && !adjacency_list.contains(from)){
        throw tmn::exception::LogicException("Error(is_connected) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }
    else if (!adjacency_list.contains(from)){
        return false;
    }

    if (strict && !adjacency_list.contains(to)){
        throw tmn::exception::LogicException("Error(is_connected) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }
    else if (!adjacency_list.contains(from)){
        return false;
    }

    return adjacency_list[from].contains(to);
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
tmn::Optional<Weight> Graph<is_oriented, VertexId, VertexType, Weight>::pass_weight(VertexId from, VertexId to) const noexcept {
    if (!adjacency_list.contains(from)){ 
        return tmn::Optional<Weight>();
    }

    if (!adjacency_list.contains(to)){
        return tmn::Optional<Weight>();
    }

    if (!adjacency_list[from].contains(to)){
        return tmn::Optional<Weight>();
    }

    return tmn::Optional<Weight>(adjacency_list[from][to].weight);
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
void Graph<is_oriented, VertexId, VertexType, Weight>::add_connected_vertices(VertexId from, VertexId to, const Weight& weight) {
    if (!adjacency_list.contains(from)){
        adjacency_list.insert(from, ConnectedVerticesList());
    }

    if (!adjacency_list.contains(to)){
        adjacency_list.insert(to, ConnectedVerticesList());
    }

    if (!adjacency_list[from].contains(to)){
        Edge new_direct_edge(from, to, weight);
        adjacency_list[from].insert({to, new_direct_edge});
    }

    if (!is_oriented){
        if (!adjacency_list[to].contains(from)){
            Edge new_back_edge(to, from, Weight{});
            adjacency_list[to].insert({from, new_back_edge});
        }
    }
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
void Graph<is_oriented, VertexId, VertexType, Weight>::reserve(std::size_t capacity) {
    resources.reserve(capacity);
    adjacency_list.reserve(capacity);
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
void Graph<is_oriented, VertexId, VertexType, Weight>::clear() {
    adjacency_list.clear();
    resources.clear();
}


template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
tmn::Pair<ArraySequence<ArraySequence<int>>, ArraySequence<VertexId>>
    Graph<is_oriented, VertexId, VertexType, Weight>::basic_adjacency_list() const noexcept {
        
    auto vertices = adjacency_list.keys();
    IntMatrix matrix(vertices.size(), ArraySequence<int>(vertices.size(), 0)); 

    HashTable<VertexId, size_t> vertex_index;
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertex_index[vertices[i]] = i;
    }

    for (const auto& vertex_pair : adjacency_list) {
        VertexId u = vertex_pair.first;
        const ConnectedVerticesList& connected_vertices = vertex_pair.second;
        size_t u_index = vertex_index[u];

        for (const auto& edge_pair : connected_vertices) {
            VertexId v = edge_pair.first;
            size_t v_index = vertex_index[v];

            matrix[u_index][v_index] = 1;
            if (!is_oriented) {
                matrix[v_index][u_index] = 1;
            }
        }
    }

    return tmn::Pair<IntMatrix, ArraySequence<VertexId>>(matrix, vertices);
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
tmn::Pair<ArraySequence<ArraySequence<Weight>>, ArraySequence<VertexId>>
    Graph<is_oriented, VertexId, VertexType, Weight>::weighted_adjacency_matrix() const noexcept {
    auto vertices = adjacency_list.keys();

    using WeightMatrix = tmn::sequence::ArraySequence<tmn::sequence::ArraySequence<Weight>>;
    WeightMatrix matrix(vertices.size(), tmn::sequence::ArraySequence<Weight>(vertices.size(), Weight{}));
    
    HashTable<VertexId, size_t> vertex_index;
    for (size_t i = 0; i < vertices.size(); ++i) {
        vertex_index[vertices[i]] = i;
    }

    for (const auto& vertex_pair : adjacency_list) {
        VertexId u = vertex_pair.first;
        const ConnectedVerticesList& connected_vertices = vertex_pair.second;
        size_t u_index = vertex_index[u];

        for (const auto& edge_pair : connected_vertices) {
            VertexId v = edge_pair.first;
            const Edge& edge = edge_pair.second;
            size_t v_index = vertex_index[v];

            matrix[u_index][v_index] = edge.weight;

            if (!is_oriented) {
                matrix[v_index][u_index] = edge.weight;
            }
        }
    }

    return tmn::Pair<WeightMatrix, ArraySequence<VertexId>>(matrix, vertices);
}

}
}

#include "Algorithms/SingleWeightedGraphAlgorithms.tpp"