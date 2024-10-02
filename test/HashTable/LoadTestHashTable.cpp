#include <gtest/gtest.h>

#include <chrono>
#include <unordered_map>

#include "../../include/Associative/HashTable.hpp"



TEST(LoadTestHashTable, HashTableInsert100000){
    const int key_block = 100000;
    const int block_count = 1000;

    double timers[key_block / block_count];
    double start_point = 0;

    tmn_associative::HashTable<int, float> htable;

    for (int i = 0; i < key_block; ++i){
        auto start = std::chrono::high_resolution_clock::now();
        htable.insert({i, static_cast<float>(i) / (i * i * i)});
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        start_point += elapsed.count();

        if ((i + 1) % block_count == 0){
            timers[(i + 1) / block_count] = start_point;
            start_point = 0;
        }
    }

    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    for (int i = 0; i < key_block / block_count; ++i){
        std::cout << i + 1 << ") " << timers[i] << " nanoseconds (time of r-value insert " <<  block_count << " pairs) [TEST_HASHTABLE]" << std::endl;
    }
}

TEST(LoadUnorderedMap, UnorderedMapInsert100000){
    const int key_block = 100000;
    const int block_count = 1000;

    double timers[key_block / block_count];
    double start_point = 0;

    std::unordered_map<int, float> umap;

    for (int i = 0; i < key_block; ++i){
        auto start = std::chrono::high_resolution_clock::now();
        umap.insert({i, static_cast<float>(i) / (i * i * i)});
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        start_point += elapsed.count();

        if ((i + 1) % block_count == 0){
            timers[(i + 1) / block_count] = start_point;
            start_point = 0;
        }
    }

    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    for (int i = 0; i < key_block / block_count; ++i){
        std::cout << i + 1 << ") " << timers[i] << " nanoseconds (time of r-value insert " <<  block_count << " pairs) [TEST_UNORDEREDMAP]" << std::endl;
    }
}