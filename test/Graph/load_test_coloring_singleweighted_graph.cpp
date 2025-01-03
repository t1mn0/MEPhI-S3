#include <gtest/gtest.h>

#include "../../include/Graph/Graph.hpp"
#include "graph_test_utils.hpp"

#include <random>
#include <algorithm>
#include <chrono>
#include <unordered_map>
#include <fstream>
#include <filesystem>

using namespace tmn::graph;

TEST(UnweightedGraphLoadTest, COLORINGTEST) {
    const int key_block = 10'000;
    const int block_count = 100;

    std::random_device rd;
    std::mt19937 gen(rd());

    double timers[key_block / block_count];
    double start_point = 0;

    Graph<true, int, std::string, complex_num> graph;

    const std::string filename = std::string(EXECUTABLE_PATH) + "/load_test_coloring_singleweighted_graph.txt";

    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    std::ofstream output_file(filename); 
    if (!output_file.is_open()) {
        FAIL() << "Could not open output file";
    }

    for (int i = 0; i < key_block; ++i) {
        graph.add_vertex(i, generate_small_string());

        std::uniform_int_distribution<> length_dist(0, i);
        std::uniform_int_distribution<> complex_gen(-i, i);

        for (int j = 0; j < i / 10; ++j){
            graph.add_edge(length_dist(gen), length_dist(gen), complex_num(complex_gen(gen), complex_gen(gen)));
        }

        if ((i + 1) % block_count == 0) {
            auto start = std::chrono::high_resolution_clock::now();
            auto ht = graph.graph_coloring();
            auto end = std::chrono::high_resolution_clock::now();

            timers[(i + 1) / block_count - 1] = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            output_file << /* (i + 1) / block_count << ") " << */ timers[(i + 1) / block_count - 1] << /* " milliseconds" << */ std::endl;
        }
    }

    output_file.close();
}