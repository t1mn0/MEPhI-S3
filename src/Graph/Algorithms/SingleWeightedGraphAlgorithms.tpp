#include "../../../include/Graph/SingleWeightGraph.hpp"

namespace tmn {
namespace graph {

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::has_cycle() const {
    HashSet<VertexId> visited;
    HashSet<VertexId> recursion_stack;

    for (const auto& pair : adjacency_list) {
        VertexId vertex = pair.first;
        if (!visited.contains(vertex)) {
            Stack<VertexId> dfs_stack;
            dfs_stack.push(vertex);
            visited.insert(vertex);
            recursion_stack.insert(vertex);

            while(!dfs_stack.empty()) {
                VertexId current_vertex = dfs_stack.top();
                bool found_neighbor = false;
                if (adjacency_list.contains(current_vertex)) {
                    for (const auto& edge_pair : adjacency_list.at(current_vertex)) {
                        if(recursion_stack.contains(edge_pair.second.to)) {
                            return true;
                        }
                        if (!visited.contains(edge_pair.second.to)) {
                            dfs_stack.push(edge_pair.second.to);
                            visited.insert(edge_pair.second.to);
                            recursion_stack.insert(edge_pair.second.to);
                            found_neighbor = true;
                            break;
                        }
                    }
                }
                if(!found_neighbor) {
                   dfs_stack.pop();
                   recursion_stack.erase(current_vertex);
                }
            }
          }
    }

    return false;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
bool Graph<is_oriented, VertexId, VertexType, Weight>::is_connected_graph() const {
    if (adjacency_list.empty()) return true;

    HashSet<VertexId> visited;
    Queue<VertexId> queue;

    VertexId start_vertex = adjacency_list.begin()->first;
    queue.push(start_vertex);
    visited.insert(start_vertex);

    while (!queue.empty()) {
        VertexId current_vertex = queue.front();
        queue.pop();

        if (adjacency_list.contains(current_vertex)) {
            for(const auto& edge_pair : adjacency_list.at(current_vertex)) {
                if (!visited.contains(edge_pair.second.to)) {
                    queue.push(edge_pair.second.to);
                    visited.insert(edge_pair.second.to);
                }
            }
         }
    }

    return visited.size() == adjacency_list.size();
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
ArraySequence<VertexId> Graph<is_oriented, VertexId, VertexType, Weight>::bfs() const {
    ArraySequence<VertexId> path;
    if (adjacency_list.empty()) return path;

    HashSet<VertexId> visited;
    visited.reserve(adjacency_list.size() * 2);

    Queue<VertexId> queue;

    VertexId start_vertex = adjacency_list.begin()->first;

    queue.push(start_vertex);
    visited.insert(start_vertex);

    while (!queue.empty()) {
        VertexId current_vertex = queue.front();
        queue.pop();
        path.push_back(start_vertex);

        if (!adjacency_list[current_vertex].empty()) {
            const auto& neighbors = adjacency_list[current_vertex];

            for (const auto& edge_pair : neighbors) {
                if ((edge_pair.first != current_vertex) && !visited.contains(edge_pair.first)) {
                    queue.push(edge_pair.first);
                    visited.insert(edge_pair.first);
                }
            }
        }

        if(visited.size() != adjacency_list.size()) {
           for(const auto& pair : adjacency_list) {
                if(current_vertex != pair.first && !visited.contains(pair.first)) {
                    queue.push(pair.first);
                    visited.insert(pair.first);
                    break;
                }
           }
        }
    }

    return path;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
ArraySequence<VertexId> Graph<is_oriented, VertexId, VertexType, Weight>::bfs(VertexId start_vertex) const {
    ArraySequence<VertexId> path;
    if (adjacency_list.empty()) return path;

    HashSet<VertexId> visited;
    visited.reserve(adjacency_list.size() * 2);

    Queue<VertexId> queue;

    if (!adjacency_list.contains(start_vertex)){
        throw tmn::exception::LogicException("Error(bfs) : vertex with such a VertexId is not in the graph: " + std::to_string(start_vertex));
    }

    queue.push(start_vertex);
    visited.insert(start_vertex);

    while (!queue.empty()) {
        VertexId current_vertex = queue.front();
        queue.pop();
        path.push_back(start_vertex);

        if (adjacency_list.contains(current_vertex)) {
            const auto& neighbors = adjacency_list[current_vertex];

            for (const auto& edge_pair : neighbors) {
                if ((edge_pair.first != current_vertex) && !visited.contains(edge_pair.first)) {
                    queue.push(edge_pair.first);
                    visited.insert(edge_pair.first);
                }
            }
        }

        if(visited.size() != adjacency_list.size()) {
           for(const auto& pair : adjacency_list) {
                if(current_vertex != pair.first && !visited.contains(pair.first)) {
                    queue.push(pair.first);
                    visited.insert(pair.first);
                    break;
                }
           }
        }
    }

    return path;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
ArraySequence<VertexId> Graph<is_oriented, VertexId, VertexType, Weight>::dfs() const {
    ArraySequence<VertexId> path;
    if (adjacency_list.empty()) return path;

    HashSet<VertexId> visited;
    visited.reserve(adjacency_list.size() * 2);

    Stack<VertexId> stack;

    VertexId start_vertex = adjacency_list.begin()->first;

    stack.push(start_vertex);
    visited.insert(start_vertex);

    while (!stack.empty()) {
        VertexId current_vertex = stack.top();
        stack.pop();
        path.push_back(current_vertex);

        if (!adjacency_list[current_vertex].empty()){
            const auto& neighbors = adjacency_list[current_vertex];

            for (const auto& edge_pair : neighbors) {
                if ((edge_pair.first != current_vertex) && (!visited.contains(edge_pair.first))) {
                    stack.push(edge_pair.first);
                    visited.insert(edge_pair.first);
                }
            }
        }

        if(visited.size() != adjacency_list.size()) {
           for(const auto& pair : adjacency_list) {
                if(current_vertex != pair.first && !visited.contains(pair.first)) {
                    stack.push(pair.first);
                    visited.insert(pair.first);
                    break;
                }
           }
        }
    }

    return path;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
ArraySequence<VertexId> Graph<is_oriented, VertexId, VertexType, Weight>::dfs(VertexId start_vertex) const {
    ArraySequence<VertexId> path;
    if (adjacency_list.empty()) return path;

    HashSet<VertexId> visited;
    visited.reserve(adjacency_list.size() * 2);

    Stack<VertexId> stack;

    if (!adjacency_list.contains(start_vertex)){
        throw tmn::exception::LogicException("Error(dfs) : vertex with such a VertexId is not in the graph: " + std::to_string(start_vertex));
    }

    stack.push(start_vertex);
    visited.insert(start_vertex);

    while (!stack.empty()) {
        VertexId current_vertex = stack.top();
        stack.pop();
        path.push_back(current_vertex);

        if (!adjacency_list[current_vertex].empty()){
            const auto& neighbors = adjacency_list[current_vertex];

            for (const auto& edge_pair : neighbors) {
                if ((edge_pair.first != current_vertex) && (!visited.contains(edge_pair.first))) {
                    stack.push(edge_pair.first);
                    visited.insert(edge_pair.first);
                }
            }
        }

        if(visited.size() != adjacency_list.size()) {
           for(const auto& pair : adjacency_list) {
                if(current_vertex != pair.first && !visited.contains(pair.first)) {
                    stack.push(pair.first);
                    visited.insert(pair.first);
                    break;
                }
           }
        }
    }

    return path;
}

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
HashTable<VertexId, int> Graph<is_oriented, VertexId, VertexType, Weight>::graph_coloring() const {
    HashTable<VertexId, int> vertex_colors;
    if (adjacency_list.empty()) return vertex_colors;

    HashSet<VertexId> all_vertices = this->all_vertices();

    for (const auto& vertex : all_vertices) {
        ArraySequence<bool> used_colors;

        for (const auto& edge_pair : adjacency_list[vertex]) {
            VertexId neighbor = edge_pair.first;
            if (vertex_colors.contains(neighbor)) {
                int neighbor_color = vertex_colors[neighbor];

                if (neighbor_color >= used_colors.size()) {
                    used_colors.resize(neighbor_color + 1, false);
                }
                used_colors[neighbor_color] = true;
            }
        }

        int color = 0;
        for (size_t i = 0; i < used_colors.size(); ++i) {
            if (used_colors[i] == false) {
                color = i;
                break;
            }
        }

        if (color == 0 && used_colors.size() != 0){
            color = used_colors.size();
        }

        vertex_colors.insert(Pair<const VertexId, int>(vertex, color));
    }

    return vertex_colors;
}

}
}