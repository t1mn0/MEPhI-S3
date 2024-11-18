#include <gtest/gtest.h>

#include <chrono>
#include <unordered_set>
#include <fstream>
#include <filesystem>

#include "../../include/Associative/HashSet.hpp"



TEST(LoadTestHashSet, HashSetInsert100000){
    const int key_block = 100000;
    const int block_count = 1000;

    double timers[key_block / block_count];
    double start_point = 0;

    // For now, the path is absolute - it's all temporary
    const std::string filename = "/home/timno/Documents/MEPhI-S3/test/_test_results/load_test_hashset.txt";

    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    std::ofstream output_file(filename); 
    if (!output_file.is_open()) {
        FAIL() << "Could not open output file";
    }

    tmn_associative::HashSet<int> hset;

    for (int i = 0; i < key_block; ++i){
        auto start = std::chrono::high_resolution_clock::now();
        hset.insert(i);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        start_point += elapsed.count();

        if ((i + 1) % block_count == 0){
            timers[(i + 1) / block_count] = start_point;
            start_point = 0;
            output_file << (i + 1) / block_count << ") " << timers[(i + 1) / block_count] 
                       << " nanoseconds (time of l-value insert " << block_count
                       << " <int>) [TEST_HASHSET]" << std::endl;
        }
    }

    output_file.close();
}

TEST(LoadUnorderedSet, UnorderedSetInsert100000){
    const int key_block = 100000;
    const int block_count = 1000;

    double timers[key_block / block_count];
    double start_point = 0;

    // For now, the path is absolute - it's all temporary
    const std::string filename = "/home/timno/Documents/MEPhI-S3/test/_test_results/load_test_unorderedset.txt";

    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    std::ofstream output_file(filename); 
    if (!output_file.is_open()) {
        FAIL() << "Could not open output file";
    }

    std::unordered_set<int> uset;

    for (int i = 0; i < key_block; ++i){
        auto start = std::chrono::high_resolution_clock::now();
        uset.insert(i);
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        start_point += elapsed.count();

        if ((i + 1) % block_count == 0){
            timers[(i + 1) / block_count] = start_point;
            start_point = 0;
            output_file << (i + 1) / block_count << ") " << timers[(i + 1) / block_count] 
                       << " nanoseconds (time of l-value insert " << block_count
                       << " <int>) [TEST_UNORDEREDSET]" << std::endl;
        }
    }

    output_file.close();
}