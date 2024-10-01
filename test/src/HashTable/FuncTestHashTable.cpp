#include <gtest/gtest.h>

#include "../../../include/Associative/HashTable.hpp"



// TEST Constructors & assignment :

TEST(HashTableTest, DefaultConstructor) {
    tmn_associative::HashTable<char, int> htable;
    ASSERT_EQ(htable.size(), 0);
    ASSERT_EQ(htable.buffer_size(), 256);
    ASSERT_EQ(htable.load_factor(), 0);
    ASSERT_EQ(htable.max_load_factor(), 0.5);
}

TEST(HashTableTest, CopyConstructor) {
    tmn_associative::HashTable<char, int> htable1;

    for (std::size_t i = 0; i < 26; ++i){
        tmn::Pair<const char, int> pair(97 + i, 97 + i); 
        htable1.insert(pair);
    }

    tmn_associative::HashTable<char, int> htable2(htable1);

    ASSERT_EQ(htable1.size(), 26);
    ASSERT_EQ(htable2.size(), 26);
    ASSERT_EQ(htable1.buffer_size(), 256);
    ASSERT_EQ(htable2.buffer_size(), 256);
    
    for (const auto& pair: htable1){
        ASSERT_EQ(pair.second, htable2.get(pair.first));
    }
}

TEST(HashTableTest, Insert) {
    tmn_associative::HashTable<char, int> htable;
    for (std::size_t i = 0; i < 10; ++i){
        tmn::Pair<const char, int> pair(65 + i, i); 
        htable.insert(pair);
    }
    ASSERT_EQ(htable.size(), 10);
    ASSERT_EQ(htable.buffer_size(), 256);
    ASSERT_EQ(htable.load_factor(), 0.0390625);
}

TEST(HashTableTest, Reserve) {
    tmn_associative::HashTable<char, int> htable;
    for (std::size_t i = 0; i < 10; ++i){
        tmn::Pair<const char, int> pair(48 + i, i); 
        htable.insert(pair);
    }
    htable.reserve(12);
    ASSERT_EQ(htable.buffer_size(), 256);
    htable.reserve(512);
    ASSERT_EQ(htable.buffer_size(), 512);
}