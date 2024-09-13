#include <gtest/gtest.h>

#include "../../include/SmartPtr/SharedPtr.hpp"

#include <vector>
#include <string>



TEST(LoadTestSharedPtr, Сreation10by100){
    std::vector<std::string*> storage_of_rawptr;
    std::vector<tmn_smrt_ptr::SharedPtr<std::string>> storage_of_sharedptr;

    std::string* raw_ptr;

    for (int i = 0; i < 10; ++i){
        raw_ptr = new std::string("string");
        storage_of_rawptr.push_back(raw_ptr);


        tmn_smrt_ptr::SharedPtr<std::string> source_ptr(raw_ptr);
        
        ASSERT_EQ(source_ptr.use_count(), 1);

        for (int j = 0; j < 100; ++j){
            tmn_smrt_ptr::SharedPtr<std::string> nonsource_ptr = source_ptr;
            ASSERT_EQ(source_ptr.use_count(), j + 2);

            storage_of_sharedptr.push_back(nonsource_ptr);
        }
    }

    ASSERT_EQ(storage_of_sharedptr.size(), 1000);
    ASSERT_EQ(storage_of_rawptr.size(), 10);

    for (int i = 0; i < 1000; i += 100){
        std::cout << storage_of_sharedptr[i].get() << std::endl;
    }

    for (int i = 0; i < 10; ++i){
        std::cout << storage_of_rawptr[i] << std::endl;
    }

    for (int i = 0; i < 1000; ++i){
        ASSERT_EQ(storage_of_sharedptr[i].get(), storage_of_rawptr[i / 100]);
    }

}