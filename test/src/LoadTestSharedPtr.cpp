#include <gtest/gtest.h>

#include "../../include/SmartPtr/SharedPtr.hpp"

#include <string>
#include <chrono>

inline double average(const double* arr, int size) {
    if (size <= 0) return 0.0;

    double sum = 0.0;
    for (int i = 0; i < size; ++i) {
        sum += arr[i];
    }

    return sum / size;
}

TEST(LoadTestSharedPtr, Сreation100by10){
    const int rawptr_count = 10;
    const int sharedptr_block = 100;

    double timers[rawptr_count];

    std::string* storage_of_rawptr[rawptr_count];
    tmn_smart_ptr::SharedPtr<std::string> storage_of_sharedptr[rawptr_count * sharedptr_block];

    for (int i = 0; i < rawptr_count; ++i){
        storage_of_rawptr[i] = new std::string("string");

        auto start = std::chrono::high_resolution_clock::now();
        tmn_smart_ptr::SharedPtr<std::string> source_ptr(storage_of_rawptr[i]);
        
        // ASSERT_EQ(source_ptr.use_count(), 1);

        for (int j = 0; j < sharedptr_block; ++j){
            tmn_smart_ptr::SharedPtr<std::string> nonsource_ptr = source_ptr;
            // ASSERT_EQ(source_ptr.use_count(), j + 2);

            storage_of_sharedptr[i * sharedptr_block + j] = nonsource_ptr;
        }

        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        timers[i] = elapsed.count();
    }

    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;

    for (int i = 0; i < rawptr_count; ++i){
        std::cout << i + 1 << ") " << storage_of_rawptr[i] << std::endl;
    }

    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;

    for (int i = 0; i < rawptr_count * sharedptr_block; i += sharedptr_block){
        std::cout << i + 1 << ") "<< storage_of_sharedptr[i].get() << std::endl;
    }

    for (int i = 0; i < rawptr_count * sharedptr_block; ++i){
        ASSERT_EQ(storage_of_sharedptr[i].get(), storage_of_rawptr[i / 100]);
    }

    double avg_time = average(timers, rawptr_count);
    double total_time = 0;
    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    for (int i = 0; i < rawptr_count; ++i){
        std::cout << i + 1 << ") " << timers[i] << " nanoseconds (time of creation 100 sharedptr by 1 rawptr)" << std::endl;
        total_time += timers[i];
    }
    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    std::cout << "Average time: " << avg_time << " nanoseconds" << std::endl;
    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    std::cout << "Total time: " << total_time << " nanoseconds" << std::endl;
    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
}