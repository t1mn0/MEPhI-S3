#include <gtest/gtest.h>

#include "../../include/SmartPtr/UniquePtr.hpp"

#include <string>
#include <chrono>



TEST(LoadTestSUniquePtr, Сreation1000){
    const int rawptr_count = 1000;

    double timer_for_rawptr;
    double timer_for_uniqueptr;

    std::string* storage_of_rawptr[rawptr_count];
    tmn_smart_ptr::UniquePtr<std::string> storage_of_uniqueptr[rawptr_count];


    for (int i = 0; i < rawptr_count; ++i){
        auto start_for_rawptr = std::chrono::high_resolution_clock::now();
        storage_of_rawptr[i] = new std::string("string");
        auto end_for_rawptr = std::chrono::high_resolution_clock::now();
        auto elapsed_for_rawptr = std::chrono::duration_cast<std::chrono::microseconds>(end_for_rawptr - start_for_rawptr);
        timer_for_rawptr += elapsed_for_rawptr.count();

        auto start_for_uniqueptr = std::chrono::high_resolution_clock::now();
        tmn_smart_ptr::UniquePtr<std::string> uniqueptr(storage_of_rawptr[i]);
        storage_of_uniqueptr[i] = std::move(uniqueptr);
        auto end_for_uniqueptr = std::chrono::high_resolution_clock::now();
        auto elapsed_for_uniqueptr = std::chrono::duration_cast<std::chrono::microseconds>(end_for_uniqueptr - start_for_uniqueptr);
        timer_for_uniqueptr += elapsed_for_uniqueptr.count();
    }

    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    std::cout << "Total time of creation 1000 string* : " << timer_for_rawptr << " microseconds" << std::endl;
    std::cout << "Total time of creation 1000 uniqueptr<string> : " << timer_for_uniqueptr << " microseconds" << std::endl;
    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
}