#include <iostream>
#include <string>
#include <sstream>

#include "GraphView.hpp"

namespace tmn::graph {

void GraphView::run() {
    bool exit = false;
    while (!exit) {
        std::string line;
        std::cout << "> ";
        std::getline(std::cin, line);

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "exit") {
            exit = true;
        }
        else if (command == "add_v") {
            std::string vertex_id_str;
            iss >> vertex_id_str;
            
            try {
                int vertex_id = std::stoi(vertex_id_str);
                add_vertex(vertex_id);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
            }
        } 
        else if (command == "rm_v") {
            std::string vertex_id_str;
            iss >> vertex_id_str;
            
            try {
                int vertex_id = std::stoi(vertex_id_str);
                remove_vertex(vertex_id);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
            }
        }
        else if (command == "connected_v") {
            std::string vertex_id_str;
            iss >> vertex_id_str;
            
            try {
                int vertex_id = std::stoi(vertex_id_str);
                connected_vertices(vertex_id);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
            }
        }
        else if (command == "pos_degree") {
            std::string vertex_id_str;
            iss >> vertex_id_str;
            
            try {
                int vertex_id = std::stoi(vertex_id_str);
                positive_vertex_degree(vertex_id);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
            }
        }
        else if (command == "add_e") {
            std::string vertex_id_str_1;
            std::string vertex_id_str_2;
            std::string weight_str;
            iss >> vertex_id_str_1 >> vertex_id_str_2 >> weight_str;
            
            int vertex_id_1, vertex_id_2;
            double weight;
            bool flag = true;

            try {
                vertex_id_1 = std::stoi(vertex_id_str_1);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
                flag = false;
            }

            try {
                vertex_id_2 = std::stoi(vertex_id_str_2);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
                flag = false;
            }

            if (!weight_str.empty()){
                try {
                    weight = std::stod(weight_str);
                } 
                catch (...) {
                    std::cerr << "Bad number for convert to weight (-> double)" << std::endl;
                    flag = false;
                }
            }
            else{
                weight = 0.0;
            }

            if (flag){
                add_edge(vertex_id_1, vertex_id_2, weight);
            }
        }
        else if (command == "rm_e") {
            std::string vertex_id_str_1;
            std::string vertex_id_str_2;
            iss >> vertex_id_str_1 >> vertex_id_str_2;
            
            int vertex_id_1, vertex_id_2;
            bool flag = true;

            try {
                vertex_id_1 = std::stoi(vertex_id_str_1);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
                flag = false;
            }

            try {
                vertex_id_2 = std::stoi(vertex_id_str_2);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
                flag = false;
            }

            if (flag){
                remove_edge(vertex_id_1, vertex_id_2);
            }
        }
        else if (command == "add_pair_v") {
            std::string vertex_id_str_1;
            std::string vertex_id_str_2;
            std::string weight_str;
            iss >> vertex_id_str_1 >> vertex_id_str_2 >> weight_str;
            
            int vertex_id_1, vertex_id_2;
            double weight;
            bool flag = true;

            try {
                vertex_id_1 = std::stoi(vertex_id_str_1);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
                flag = false;
            }

            try {
                vertex_id_2 = std::stoi(vertex_id_str_2);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
                flag = false;
            }

            if (!weight_str.empty()){
                try {
                    weight = std::stod(weight_str);
                } 
                catch (...) {
                    std::cerr << "Bad number for convert to weight (-> double)" << std::endl;
                    flag = false;
                }
            }
            else{
                weight = 0.0;
            }

            if (flag){
                add_connected_vertices(vertex_id_1, vertex_id_2, weight);
            }
        }
        else if (command == "adj_list") {
            adjacency_list();
        }
        else if (command == "has_cycle") {
            has_cycle();
        }
        else if (command == "is_connected") {
            is_connected_graph();
        }
        else if (command == "coloring") {
            coloring();
        }
        else if (command == "shortest_path") {
            std::string vertex_id_str_1;
            std::string vertex_id_str_2;
            iss >> vertex_id_str_1 >> vertex_id_str_2;
            
            int vertex_id_1, vertex_id_2;
            bool flag = true;

            try {
                vertex_id_1 = std::stoi(vertex_id_str_1);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
                flag = false;
            }

            try {
                vertex_id_2 = std::stoi(vertex_id_str_2);
            } 
            catch (...) {
                std::cerr << "Bad number for convert to vertex_id (-> int)" << std::endl;
                flag = false;
            }

            if (flag){
                find_shortest_path(vertex_id_1, vertex_id_2);
            }
        }
        else if (command == "update_viz") {
            update_visualization();
        }
        else if (command == "help") {
            std::string command_name;
            iss >> command_name;
            if (command_name.empty()){
                global_help();
            }
            else if (command_name == "add_v"){
                add_vertex_help();
            }
            else if (command_name == "rm_v"){
                remove_vertex_help();
            }
            else if (command_name == "connected_v"){
                connected_vertices_help();
            }
            else if (command_name == "pos_degree"){
                positive_vertex_degree_help();
            }
            else if (command_name == "add_e"){
                add_edge_help();
            }
            else if (command_name == "rm_e"){
                remove_edge_help();
            }
            else if (command_name == "add_pair_v"){
                add_connected_vertices_help();
            }
            else if (command_name == "adj_list"){
                adjacency_list_help();
            }
            else if (command_name == "has_cycle"){
                has_cycle_help();
            }
            else if (command_name == "is_connected"){
                is_connected_graph_help();
            }
            else if (command_name == "coloring"){
                coloring_help();
            }
            else if (command_name == "shortest_path"){
                find_shortest_path_help();
            }
            else if (command_name == "update_viz"){
                update_visualization_help();
            }
            else if (command_name == "exit"){
                exit_help();
            }

        }
        else {
            std::cout << "Wrong command" << std::endl;
        }
    }
}

void GraphView::clear() const noexcept {
    std::cout << "\x1B[2J\x1B[H" << std::endl;
}

}