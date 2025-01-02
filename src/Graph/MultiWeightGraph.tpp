#include <utility>

#include "../../include/Graph/MultiWeightGraph.hpp"

namespace tmn {
namespace graph {

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::Graph(const Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>& other) noexcept :
    adjacency_list(other.adjacency_list), resources(other.resources) {}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::Graph(Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>&& other) noexcept :
    adjacency_list(std::move(other.adjacency_list)), resources(std::move(other.resources)) {
        other.adjacency_list.clear();
        other.resources.clear();
    }

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>& 
    Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::operator=(const Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>& other_graph) {

        adjacency_list = other_graph.adjacency_list;
        resources = other_graph.resources;

        return *this;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>& 
    Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::operator=(Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>&& other_graph) {

        adjacency_list = std::move(other_graph.adjacency_list);
        resources = std::move(other_graph.resources);

        other_graph.adjacency_list.clear();
        other_graph.resources.clear();

        return *this;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::~Graph() {
    adjacency_list.clear();
    resources.clear();
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
bool Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::oriented() const noexcept {
    return is_oriented;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
bool Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::add_vertex(VertexId vertex_id) noexcept {
    if (adjacency_list.contains(vertex_id)){
        return false;
    }

    adjacency_list.insert({vertex_id, ConnectedVerticesList()});
    return true;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
bool Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::add_vertex(VertexId vertex_id, const VertexType& vertex_resource, bool strict) {
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

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
bool Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::add_vertex(VertexId vertex_id, VertexType&& vertex_resource, bool strict) {
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

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
bool Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::remove_vertex(VertexId vertex_id, bool strict) {
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

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
bool Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::vertex_in_graph(VertexId vertex_id) const noexcept {
    return adjacency_list.contains(vertex_id);
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
HashSet<VertexId> Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::all_vertices() const noexcept {
    HashSet<VertexId> result;
    for (auto& v : adjacency_list){
        result.insert(v.first);
    }
    return result;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
std::size_t Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::v_size() const noexcept {
    return adjacency_list.size();
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
tmn::Optional<std::size_t> Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::connected_vertices_count(VertexId vertex_id) const noexcept {
    if (!adjacency_list.contains(vertex_id)){
        return tmn::Optional<std::size_t>();
    }
    return tmn::Optional<std::size_t>(adjacency_list[vertex_id].size());
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
HashSet<VertexId> Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::connected_vertices(VertexId vertex_id) const {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(connected_vertices) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }
    HashSet<VertexId> vertices;
    for (const auto& vertex_id : adjacency_list[vertex_id]){
        vertices.insert(vertex_id.first);
    }
    return vertices;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
const VertexType& Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::get_resource(VertexId vertex_id) const {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(get_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    if (!resources.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(get_resource) : vertex with such a VertexId does not contain a resource: " + std::to_string(vertex_id));
    }

    return resources[vertex_id];
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
VertexType& Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::get_resource(VertexId vertex_id) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(get_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    if (!resources.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(get_resource) : vertex with such a VertexId does not contain a resource: " + std::to_string(vertex_id));
    }

    return resources[vertex_id];
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
void Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::change_vertex_resource(VertexId vertex_id, const VertexType& vertex_resource) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(change_vertex_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    resources[vertex_id] = vertex_resource;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
void Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::change_vertex_resource(VertexId vertex_id, VertexType&& vertex_resource) {
    if (!adjacency_list.contains(vertex_id)){
        throw tmn::exception::LogicException("Error(change_vertex_resource) : vertex with such a VertexId is not in the graph: " + std::to_string(vertex_id));
    }

    resources[vertex_id] = std::move(vertex_resource);
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
bool Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::change_weight_calculation_function(std::function<ResultWeight(WeightTypes...)> new_function) noexcept {
    if (new_function) {
        weight_calculation_function = new_function;
        return true;
    }

    return false;
}

/// FROM THIS

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename... WeightTypes>
void Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::add_edge(VertexId from, VertexId to, const WeightTypes&... weight_package) {
    if (!adjacency_list.contains(from)){
        throw tmn::exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }

    if (!adjacency_list.contains(to)){
        throw tmn::exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }

    Edge new_direct_edge(from, to, weight_package...);

    adjacency_list[from].insert({to, new_direct_edge});

    if (!is_oriented){
        Edge new_back_edge(to, from, weight_package...);
        adjacency_list[to].insert({from, new_back_edge});
    }
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
void Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::add_edge(VertexId from, VertexId to, WeightTypes&& ... weight_package){
    if (!adjacency_list.contains(from)){
        throw tmn::exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(from));
    }

    if (!adjacency_list.contains(to)){
        throw tmn::exception::LogicException("Error(add_edge) : vertex with such a VertexId is not in the graph: " + std::to_string(to));
    }

    Edge new_direct_edge(from, to, std::forward(weight_package)...);

    adjacency_list[from].insert({to, new_direct_edge});

    if (!is_oriented){
        Edge new_back_edge(to, from, new_direct_edge.weights);
        adjacency_list[to].insert({from, new_back_edge});
    }
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
bool Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::remove_edge(VertexId from, VertexId to, bool strict) {
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

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
bool Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::is_connected(VertexId from, VertexId to, bool strict) const {
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

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename... WeightTypes>
tmn::Optional<ResultWeight> Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::calculate_pass_weight(VertexId from, VertexId to) const noexcept {
    if (!adjacency_list.contains(from)){ 
        return tmn::Optional<ResultWeight>();
    }

    if (!adjacency_list.contains(to)){
        return tmn::Optional<ResultWeight>();
    }

    if (!adjacency_list[from].contains(to)){
        return tmn::Optional<ResultWeight>();
    }

    return tmn::Optional<ResultWeight>(weight_calculation_function(adjacency_list[from][to].weights));
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
void Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::reserve(std::size_t capacity) {
    resources.reserve(capacity);
    adjacency_list.reserve(capacity);
}

template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
void Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::clear() {
    adjacency_list.clear();
    resources.clear();
}


template <bool is_oriented, typename VertexId, typename VertexType, typename ResultWeight, typename ... WeightTypes>
tmn::Pair<ArraySequence<ArraySequence<int>>, ArraySequence<VertexId>>
    Graph<is_oriented, VertexId, VertexType, ResultWeight, WeightTypes...>::basic_adjacency_list() const noexcept {
        
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

}
}