#include <gtest/gtest.h>

#include <string>
#include <chrono>

#include "../../include/SmartPtr/SharedPtr.hpp"



TEST(LoadTestSharedPtr, Сreation1000by100){
    const int rawptr_count = 100;
    const int sharedptr_block = 100;

    double timers[sharedptr_block];

    tmn_smart_ptr::SharedPtr<std::string> storage_of_sharedptr[rawptr_count * sharedptr_block];

    for (int i = 0; i < rawptr_count; ++i){
        auto start = std::chrono::high_resolution_clock::now();
        tmn_smart_ptr::SharedPtr<std::string> source_ptr(new std::string("string"));

        for (int j = 0; j < sharedptr_block; ++j){
            tmn_smart_ptr::SharedPtr<std::string> nonsource_ptr = source_ptr;
            storage_of_sharedptr[i * sharedptr_block + j] = nonsource_ptr;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        timers[i] = elapsed.count();
    }

    double total_time = 0;
    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    for (int i = 0; i < rawptr_count; ++i){
        std::cout << i + 1 << ") " << timers[i] << " nanoseconds (time of creation " <<  sharedptr_block << " sharedptr by 1 rawptr) [TEST_SHARED]" << std::endl;
        total_time += timers[i];
    }
    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    std::cout << "Total time: " << total_time << " nanoseconds" << std::endl;
    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
}