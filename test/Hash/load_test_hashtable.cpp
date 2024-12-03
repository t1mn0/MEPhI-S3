#include <gtest/gtest.h>

#include <chrono>
#include <unordered_map>
#include <fstream>
#include <filesystem>

#include "../../include/Associative/HashTable.hpp"

TEST(LoadTestHashTable, Insert) {
    const int key_block = 1000000;
    const int block_count = 1000;

    double timers[key_block / block_count];
    double start_point = 0;

    tmn_associative::HashTable<int, float> htable;

    // For now, the path is absolute - it's all temporary
    const std::string filename = std::string(EXECUTABLE_PATH) + "/load_test_hashtable.txt";

    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    std::ofstream output_file(filename); 
    if (!output_file.is_open()) {
        FAIL() << "Could not open output file";
    }

    for (int i = 0; i < key_block; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        htable.insert({i, static_cast<float>(i) / (i * i * i)});
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        start_point += elapsed.count();

        if ((i + 1) % block_count == 0) {
            timers[(i + 1) / block_count] = start_point;
            start_point = 0;
            output_file << (i + 1) / block_count << ") " << timers[(i + 1) / block_count] 
                       << " nanoseconds (time of r-value insert " << block_count
                       << " pairs) [TEST_HASHTABLE]" << std::endl;
        }
    }

    output_file.close();
}

TEST(LoadTestUnorderedMap, Insert) {
    const int key_block = 1000000;
    const int block_count = 1000;

    double timers[key_block / block_count];
    double start_point = 0;

    std::unordered_map<int, float> htable;

    // For now, the path is absolute - it's all temporary
    const std::string filename = std::string(EXECUTABLE_PATH) + "/load_test_unorderedmap.txt";

    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }

    std::ofstream output_file(filename); 
    if (!output_file.is_open()) {
        FAIL() << "Could not open output file";
    }

    for (int i = 0; i < key_block; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        htable.insert({i, static_cast<float>(i) / (i * i * i)});
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        start_point += elapsed.count();

        if ((i + 1) % block_count == 0) {
            timers[(i + 1) / block_count] = start_point;
            start_point = 0;
            output_file << (i + 1) / block_count << ") " << timers[(i + 1) / block_count] 
                       << " nanoseconds (time of r-value insert " << block_count
                       << " pairs) [TEST_UNORDEREDMAP]" << std::endl;
        }
    }

    output_file.close();
}