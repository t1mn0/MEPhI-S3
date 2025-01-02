#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>

#include "../../../include/Graph/Graph.hpp"

namespace tmn {
namespace graph {

struct VertexResource {
    sf::Vector2f position;
    sf::Color color;
};

template <bool is_oriented, typename VertexId, typename VertexType, typename Weight>
class GraphVisualizer {
private:
    Graph<is_oriented, VertexId, VertexType, Weight> graph;

    sf::RenderWindow window;
    const float node_radius = 20.f;

public:
    GraphVisualizer(const sf::VideoMode& mode, const std::string& title) : window(mode, title) {}

    void set_graph(const Graph<is_oriented, VertexId, VertexType, Weight>& g) {
        graph = g;
        update_visualization();
    }

    void update_visualization() {
        for (const auto& [vertex_id, _] : graph.adjacency_list) {
            sf::CircleShape shape(node_radius);
            shape.setPosition(graph.get_resource(vertex_id).position); 
            shape.setFillColor(graph.get_resource(vertex_id).color);
        }

        for (const auto& [from, edges] : graph.adjacency_list) {
            for (const auto& [to, edge] : edges) {
                EdgeData edge_data;
                edge_data.line = sf::VertexArray(sf::Lines, 2);
                edge_data.line[0].position = vertex_map[from].shape.getPosition();
                edge_data.line[1].position = vertex_map[to].shape.getPosition();
                
            }
        }
    }

    void draw() {
        window.clear(sf::Color::White);

        for (const auto& [_, vertex_data] : vertex_map) {
            window.draw(vertex_data.shape);
        }

        for (const auto& edge_data : edge_list) {
            window.draw(edge_data.line);
            // Optionally draw weight labels (if Weight is not void)
            if constexpr (!std::is_same_v<Weight, void>) { 
                // ... (Draw weight label near the edge)
            }
        }

        window.display();
    }

    // ... (Implement methods for adding/removing vertices/edges, 
    //      changing weights, visualizing algorithms, etc.)

private:
    sf::Vector2f getRandomPosition() { 
        // Generate random position within the window bounds
        return sf::Vector2f(
            static_cast<float>(rand() % window.getSize().x), 
            static_cast<float>(rand() % window.getSize().y)
        );
    }
};

// Specialization for Graph<is_oriented, VertexId, VertexType, void>
template <bool is_oriented, typename VertexId, typename VertexType>
class GraphVisualizer<is_oriented, VertexId, VertexType, void> {
    // ... (Similar implementation as above, but remove Weight from EdgeData
    //      and related code since there is no weight in this specialization)
};

}
}

// ... (Example usage)

int main() {
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::MouseButtonPressed) {
                handleMouseEvents();
            }
        }

        // Обновляем визуализацию
        update_visualization();

        // Рисуем граф
        draw();
    }

    return 0;
}