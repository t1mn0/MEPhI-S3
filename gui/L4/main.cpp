#include <iostream>

#include "GraphView.hpp"

int suggest_graph_type() {
    std::cout << "You can initialize the graph using one of the suggested types or start without initialization. Enter the selection:" << std::endl;
    std::cout << "1. Make complete graph" << std::endl;
    std::cout << "2. Make bipartite complete graph" << std::endl;
    std::cout << "3. Make `simple-chain` graph" << std::endl;
    std::cout << "4. Make `wheel` graph" << std::endl;
    std::cout << "5. Make biconnected graph" << std::endl;
    std::cout << "0. Start without initialization (empty graph)" << std::endl;
    
    unsigned int attempts = 5;
    int choice = -1; 
    
    while (attempts != 0){
        std::cin >> choice;
        std::cout << std::endl;
        if (choice > 5 || choice < 0){
            --attempts;
        }
        else{
            return choice;
        }
    }

    return -1;
}

int get_graph_size() {
    std::cout << "Enter the graph size:" << std::endl;
    unsigned int attempts = 5;
    int choice = -1; 
    
    while (attempts != 0){
        std::cin >> choice;
        std::cout << std::endl;
        if (choice > 256 || choice < 1){
            --attempts;
        }
        else{
            return choice;
        }
    }

    return -1;
}

tmn::Pair<int,int> get_parts_size() {
    std::cout << "Enter the size of the first part of the graph:" << std::endl;
    unsigned int attempts = 5;
    int choice1 = -1; 
    
    while (attempts != 0){
        std::cin >> choice1;
        std::cout << std::endl;
        if (choice1 > 256 || choice1 < 1){
            --attempts;
        }
        else{
            break;
        }
    }

    if (attempts == 0){
        return {-1, -1};
    }

    attempts = 5;

    std::cout << "Enter the size of the second part of the graph:" << std::endl;
    int choice2 = -1; 
    
    while (attempts != 0){
        std::cin >> choice2;
        std::cout << std::endl;
        if (choice2 > 256 || choice2 < 1){
            --attempts;
        }
        else{
            return {choice1, choice2};
        }
    }

    return {-1, -1};
}

int main(){
    int graph_type = suggest_graph_type();
    if (graph_type == -1){
        exit(0);
    }
    else{
        int v_size = -1;
        tmn::graph::GraphView view;
        switch (graph_type) {
        case 1:
            v_size = get_graph_size();
            if (v_size == -1){
                exit(0);
            }
            view.init_complete_graph(v_size);
            break;
        case 2 : {
            tmn::Pair<int, int> parts = get_parts_size();
            if (parts.first == -1 && parts.second == -1){
                exit(0);
            }
            view.init_bipartite_complete_graph(parts.first, parts.second);
            break;
        }
        case 3:
            v_size = get_graph_size();
            if (v_size == -1){
                exit(0);
            }
            view.init_simple_chain_graph(v_size);
            break;
        case 4:
            v_size = get_graph_size();
            if (v_size == -1){
                exit(0);
            }
            view.init_wheel_graph(v_size);
            break;
        case 5:
            v_size = get_graph_size();
            if (v_size == -1){
                exit(0);
            }
            view.init_biconnected_graph(v_size);
            break;
        default:
            break;
        }
    
        getchar();
        view.run();
    }

    return 0;
}