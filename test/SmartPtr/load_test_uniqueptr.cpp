#include <gtest/gtest.h>

#include <string>
#include <chrono>
#include <memory>

#include "../../include/SmartPtr/UniquePtr.hpp"



TEST(LoadTestSUniquePtr, Сreation100000){
    const int rawptr_count = 100000;
    const int block_count = 1000;

    double timer_for_rawptr = 0;
    double timer_for_uniqueptr = 0;
    double timer_for_stl_uniqueptr = 0;

    double timers1[rawptr_count / block_count];
    double timers2[rawptr_count / block_count];
    double timers3[rawptr_count / block_count];

    double start_point_1 = 0;
    double start_point_2 = 0;
    double start_point_3 = 0;

    for (int i = 0; i < rawptr_count; ++i){
        auto start_for_rawptr = std::chrono::high_resolution_clock::now();
        std::string* tester1 = new std::string("string");
        auto end_for_rawptr = std::chrono::high_resolution_clock::now();
        auto elapsed_for_rawptr = std::chrono::duration_cast<std::chrono::nanoseconds>(end_for_rawptr - start_for_rawptr);
        start_point_1 += elapsed_for_rawptr.count();
        delete tester1;

        auto start_for_uniqueptr = std::chrono::high_resolution_clock::now();
        tmn::smart_ptr::UniquePtr<std::string> tester2 = tmn::smart_ptr::make_unique<std::string>("string");
        auto end_for_uniqueptr = std::chrono::high_resolution_clock::now();
        auto elapsed_for_uniqueptr = std::chrono::duration_cast<std::chrono::nanoseconds>(end_for_uniqueptr - start_for_uniqueptr);
        start_point_2 += elapsed_for_uniqueptr.count();

        auto start_for_stl_uniqueptr = std::chrono::high_resolution_clock::now();
        std::unique_ptr<std::string> tester3 = std::make_unique<std::string>("string");
        auto end_for_stl_uniqueptr = std::chrono::high_resolution_clock::now();
        auto elapsed_for_stl_uniqueptr = std::chrono::duration_cast<std::chrono::nanoseconds>(end_for_stl_uniqueptr - start_for_stl_uniqueptr);
        start_point_3 += elapsed_for_stl_uniqueptr.count();

        if ((i + 1) % block_count == 0){
            timers1[(i + 1) / block_count] = start_point_1;
            timers2[(i + 1) / block_count] = start_point_2;
            timers3[(i + 1) / block_count] = start_point_3;
            start_point_1 = 0;
            start_point_2 = 0;
            start_point_3 = 0;
        }
    }

    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    for (int i = 0; i < rawptr_count / block_count; ++i){
        std::cout << i + 1 << ") " << timers1[i] << " nanoseconds (time of creation " <<  block_count << " string*) [TEST_UNIQUE]" << std::endl;
        timer_for_rawptr += timers1[i];
        timer_for_rawptr /= 1000;
    }

    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    for (int i = 0; i < rawptr_count / block_count; ++i){
        std::cout << i + 1 << ") " << timers2[i] << " nanoseconds (time of creation " <<  block_count << " uniqueptr) [TEST_UNIQUE]" << std::endl;
        timer_for_uniqueptr += timers2[i];
        timer_for_uniqueptr /= 1000;
    }
    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    for (int i = 0; i < rawptr_count / block_count; ++i){
        std::cout << i + 1 << ") " << timers3[i] << " nanoseconds (time of creation " <<  block_count << " stl_uniqueptr) [TEST_UNIQUE]" << std::endl;
        timer_for_stl_uniqueptr += timers3[i];
        timer_for_stl_uniqueptr /= 1000;
    }


    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
    std::cout << "Total time of creation " << rawptr_count << " string* : " << timer_for_rawptr << " microseconds [TEST_UNIQUE]" << std::endl;
    std::cout << "Total time of creation " << rawptr_count << " uniqueptr<string> : " << timer_for_uniqueptr << " microseconds [TEST_UNIQUE]" << std::endl;
    std::cout << "Total time of creation " << rawptr_count << " stl_uniqueptr<string> : " << timer_for_stl_uniqueptr << " microseconds [TEST_UNIQUE]" << std::endl;
    std::cout << "- - - - - - - - - - - - - - - - - - - - " << std::endl;
}
