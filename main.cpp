#include "include/Graph/Graph.hpp"

#include <iostream>

int main(){
    tmn::graph::Graph<false, int, char, void> graph = { 
        {0, {2, 4, 6}},
        {1, {3, 5, 2}   }
    };


    std::cout <<  graph.is_connected_graph() << std::endl;
}