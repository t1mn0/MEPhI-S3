#include "../../../include/Graph/UnweightedGraph.hpp"

namespace tmn {
namespace graph {

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::has_cycle() const {
    if (adjacency_list.empty()) return false;

    HashSet<VertexId> visited;
    HashSet<VertexId> recursion_stack;

    Function<bool(VertexId)> dfs = [&](VertexId vertex) {
        visited.insert(vertex);
        recursion_stack.insert(vertex);

         if (adjacency_list.contains(vertex)) {
            for (const auto& neighbor : adjacency_list[vertex]) {
                if (!visited.contains(neighbor)) {
                   if(dfs(neighbor)) return true;
                } 
                else if (recursion_stack.contains(neighbor)) {
                   return true;
                }
            }
        }
        recursion_stack.erase(vertex);
        return false;
    };

    for (const auto& pair : adjacency_list) {
        if (!visited.contains(pair.first)) {
            if(dfs(pair.first)) {
                return true;
            }
        }
    }

    return false;
}

template <bool is_oriented, typename VertexId, typename VertexType>
bool Graph<is_oriented, VertexId, VertexType, void>::is_connected_graph() const {
    if (adjacency_list.empty()) return true;

    HashSet<VertexId> visited;
    Queue<VertexId> queue;

    VertexId start_vertex = adjacency_list.begin()->first;
    queue.push(start_vertex);
    visited.insert(start_vertex);

    while(!queue.empty()) {
        VertexId current_vertex = queue.front();
        queue.pop();
        if (adjacency_list.contains(current_vertex)) {
                for(const auto& neighbor : adjacency_list[current_vertex]) {
                if(!visited.contains(neighbor)) {
                    queue.push(neighbor);
                    visited.insert(neighbor);
                }
            }
        }
    }

    return visited.size() == adjacency_list.size();
}

template <bool is_oriented, typename VertexId, typename VertexType>
ArraySequence<VertexId> Graph<is_oriented, VertexId, VertexType, void>::bfs() const {
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
        path.push_back(current_vertex);

        if (!adjacency_list[current_vertex].empty()){
            const auto& neighbors = adjacency_list[current_vertex];

            for (const auto& neighbor : neighbors) {
                if ((neighbor != current_vertex) && (!visited.contains(neighbor))) {
                    queue.push(neighbor);
                    visited.insert(neighbor);
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

template <bool is_oriented, typename VertexId, typename VertexType>
ArraySequence<VertexId> Graph<is_oriented, VertexId, VertexType, void>::bfs(VertexId start_vertex) const {
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
        path.push_back(current_vertex);
        
        visited.insert(current_vertex);

        if (!adjacency_list[current_vertex].empty()){
            const auto& neighbors = adjacency_list[current_vertex];

            for (const auto& neighbor : neighbors) {
                if ((neighbor != current_vertex) && (!visited.contains(neighbor))) {
                    queue.push(neighbor);
                    visited.insert(neighbor);
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

template <bool is_oriented, typename VertexId, typename VertexType>
ArraySequence<VertexId> Graph<is_oriented, VertexId, VertexType, void>::dfs() const {
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

        visited.insert(current_vertex);

        if (!adjacency_list[current_vertex].empty()){
            const auto& neighbors = adjacency_list[current_vertex];

            for (const auto& neighbor : neighbors) {
            if ((neighbor != current_vertex) && (!visited.contains(neighbor))) {
                    stack.push(neighbor);
                    visited.insert(current_vertex);
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

template <bool is_oriented, typename VertexId, typename VertexType>
ArraySequence<VertexId> Graph<is_oriented, VertexId, VertexType, void>::dfs(VertexId start_vertex) const {
    ArraySequence<VertexId> path;

    if (adjacency_list.empty()) return path;

    HashSet<VertexId> visited;
    visited.reserve(adjacency_list.size());

    Stack<VertexId> stack;

    if (!adjacency_list.contains(start_vertex)){
        throw tmn::exception::LogicException("Error(dfs) : vertex with such a VertexId is not in the graph: " + std::to_string(start_vertex));
    }

    stack.push(start_vertex);

    while (!stack.empty()) {
        VertexId current_vertex = stack.top();
        stack.pop();

        if (!adjacency_list[current_vertex].empty()){
            visited.insert(current_vertex);
            path.push_back(current_vertex);

            const auto& neighbors = adjacency_list[current_vertex];
            for (const auto& neighbor : neighbors) {
                if ((neighbor != current_vertex) && (!visited.contains(neighbor))) {
                    stack.push(neighbor);
                }
            }
        }

        if(visited.size() != adjacency_list.size()) {
           for(const auto& pair : adjacency_list) {
                if(!visited.contains(pair.first)) {
                    stack.push(pair.first);
                    break;
                }
           }
        }
    }
    
    return path;
}

template <bool is_oriented, typename VertexId, typename VertexType>
HashTable<VertexId, int> Graph<is_oriented, VertexId, VertexType, void>::graph_coloring() const {
    HashTable<VertexId, int> vertex_colors;
    if (adjacency_list.empty()) return vertex_colors;

    HashSet<VertexId> all_vertices = this->all_vertices();

    for (const auto& vertex : all_vertices) {
        ArraySequence<bool> used_colors;

        const auto& neighbors = adjacency_list[vertex];

        for (const auto& neighbor : neighbors) {
            if (vertex_colors.contains(neighbor)) {
                int neighbor_color = vertex_colors[neighbor];
                
                if(neighbor_color >= used_colors.size()) {
                    used_colors.resize(neighbor_color + 1, false);
                }

                used_colors[neighbor_color] = true;
            }
        }

        int color = 0;
        for(size_t i = 0; i < used_colors.size(); ++i) {
            if (used_colors[i] == false) {
                color = i;
                break;
            }
        }
        
        if (color == 0 && used_colors.size() != 0){
            color = used_colors.size() + 1;
        }

        vertex_colors.insert({vertex, color});
    }

    return vertex_colors;
}

}
}