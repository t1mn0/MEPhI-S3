#include <gtest/gtest.h>

#include <cctype>

#include "../../../include/Associative/HashSet.hpp"



// TEST Constructors & assignment :

TEST(HashSetTest, DefaultConstructor) {
    tmn_associative::HashSet<int> hset;
    ASSERT_EQ(hset.size(), 0);
    ASSERT_EQ(hset.buffer_size(), 32);
    ASSERT_EQ(hset.load_factor(), 0);
    ASSERT_EQ(hset.max_load_factor(), 0.5);
}

TEST(HashSetTest, CopyConstructor) {
    tmn_associative::HashSet<int> hset1;

    for (std::size_t i = 0; i < 26; ++i){
        hset1.insert(-(i * 101));
    }

    tmn_associative::HashSet<int> hset2(hset1);

    ASSERT_EQ(hset1.size(), 26);
    ASSERT_EQ(hset2.size(), 26);
    ASSERT_EQ(hset1.buffer_size(), 64);
    ASSERT_EQ(hset2.buffer_size(), 64);
}

TEST(HashSetTest, MoveConstructor) {
    tmn_associative::HashSet<int> hset1;

    for (std::size_t i = 0; i < 26; ++i){
        hset1.insert(-(i * 101));
    }

    tmn_associative::HashSet<int> hset2(std::move(hset1));

    ASSERT_EQ(hset1.size(), 0);
    ASSERT_EQ(hset2.size(), 26);
    ASSERT_EQ(hset1.buffer_size(), 0);
    ASSERT_EQ(hset2.buffer_size(), 64);
}

TEST(HashSetTest, InitializerListConstructor) {
    tmn_associative::HashSet<char> hset = {'a', 'a', 'c', 'b', 'b', 'd', 'c', 'b'};

    ASSERT_EQ(hset.size(), 4);
    ASSERT_EQ(hset.buffer_size(), 32);
}

TEST(HashSetTest, Swap) {
    tmn_associative::HashSet<char> hset1;
    tmn_associative::HashSet<char> hset2;

    for (std::size_t i = 0; i < 26; ++i){
        hset1.insert(char(97 + i));
    }

    for (std::size_t i = 0; i < 13; ++i){
        hset2.insert(char(65 + i));
    } 

    hset1.swap(hset2);

    ASSERT_EQ(hset1.size(), 13);
    ASSERT_EQ(hset2.size(), 26);

    for (const auto& element: hset1){
        ASSERT_FALSE(std::islower(element));
    }

    for (const auto& element: hset2){
        ASSERT_TRUE(std::islower(element));
    }
}

TEST(HashSetTest, CopyAssignment) {
    tmn_associative::HashSet<char> hset1;
    tmn_associative::HashSet<char> hset2;

    for (std::size_t i = 0; i < 26; ++i){
        hset1.insert(char(97 + i));
    }

    for (std::size_t i = 0; i < 13; ++i){
        hset2.insert(char(65 + i));
    } 

    hset1 = hset2;

    ASSERT_EQ(hset1.size(), 13);
    ASSERT_EQ(hset2.size(), 13);

    for (const auto& element: hset1){
        ASSERT_TRUE(hset2.contains(element));
    }
}

TEST(HashSetTest, MoveAssignment) {
    tmn_associative::HashSet<char> hset1;
    tmn_associative::HashSet<char> hset2;

    for (std::size_t i = 0; i < 26; ++i){
        hset1.insert(char(97 + i));
    }

    for (std::size_t i = 0; i < 13; ++i){
        hset2.insert(char(65 + i));
    } 

    hset1 = std::move(hset2);

    ASSERT_EQ(hset1.size(), 13);
    ASSERT_EQ(hset2.size(), 0);
}

TEST(HashSetTest, GetFields) {
    tmn_associative::HashSet<int> hset;

    for (std::size_t i = 0; i < 123; ++i){
        hset.insert(i);
    }

    ASSERT_EQ(hset.size(), 123);
    ASSERT_FALSE(hset.empty());
    ASSERT_EQ(hset.buffer_size(), 256);
    ASSERT_EQ(hset.max_load_factor(), 0.5);

    std::cout << "\nCURRENT LOAD FACTOR: " << hset.load_factor() << "\n\n";
}

TEST(HashSetTest, Insert) {
    tmn_associative::HashSet<int> hset;
    for (std::size_t i = 1; i < 11; ++i){
        int k = i * i * i; 
        hset.insert(k);
    }
    ASSERT_EQ(hset.size(), 10);
    ASSERT_EQ(hset.buffer_size(), 32);
    for (std::size_t i = 6; i < 11; ++i){
        int k = i * i * i; 
        hset.insert(k);
    }
    ASSERT_EQ(hset.size(), 10);
    ASSERT_EQ(hset.buffer_size(), 32);
}

TEST(HashSetTest, Insert512) {
    tmn_associative::HashSet<float> hset;
    for (std::size_t i = 0; i < 512; ++i){
        hset.insert(static_cast<float>(i) / 100);
    }
    ASSERT_EQ(hset.size(), 512);
    std::cout << "\nCURRENT BUFFER SIZE: " << hset.buffer_size() << "\n";
    std::cout << "\nCURRENT LOAD FACTOR: " << hset.load_factor() << "\n\n";
    for (std::size_t i = 412; i < 512; ++i){
        hset.insert(static_cast<float>(i) / 100);
    }
    ASSERT_EQ(hset.size(), 512);
    hset.insert(static_cast<float>(-1));
    ASSERT_EQ(hset.size(), 513);
    ASSERT_EQ(hset.buffer_size(), 2048);
}

TEST(HashSetTest, EraseRvalue) {
    tmn_associative::HashSet<char> hset;
    for (std::size_t i = 0; i < 26; ++i){
        char c = char(65 + i); 
        hset.insert(c);
    }
    for (std::size_t i = 0; i < 10; ++i){
        ASSERT_TRUE(hset.erase(75 + i));
    }
    ASSERT_EQ(hset.size(), 16);
    ASSERT_FALSE(hset.erase(48));
    ASSERT_FALSE(hset.erase(80));
    ASSERT_FALSE(hset.erase(91));
    ASSERT_FALSE(hset.erase(64));
}

TEST(HashSetTest, Clear) {
    tmn_associative::HashSet<char> hset = {'Q', 'W', 'E', 'R', 'T', 'Y' };

    ASSERT_EQ(hset.size(), 6);
    ASSERT_EQ(hset.buffer_size(), 32);
    hset.clear();
    ASSERT_EQ(hset.size(), 0);
    ASSERT_EQ(hset.buffer_size(), 32);
}

TEST(HashSetTest, Contains) {
    tmn_associative::HashSet<char> hset = {'Q', 'W', 'E', 'R', 'T', 'Y' };
    
    ASSERT_FALSE(hset.contains('A'));
    ASSERT_FALSE(hset.contains('@'));
    ASSERT_FALSE(hset.contains('~'));
    ASSERT_FALSE(hset.contains('}'));
    ASSERT_TRUE(hset.contains('Y'));
    ASSERT_TRUE(hset.contains('Q'));
}

TEST(HashSetTest, ToSequence) {
    tmn_associative::HashSet<char> hset = {'Q', 'W', 'E', 'R', 'T', 'Y',  '&', 'D', 'V', 'O', 'R', 'A', 'K'};

    tmn_sequence::ArraySequence<char> elements = hset.to_sequence();
    ASSERT_EQ(elements.size(), 12);
    
    std::cout << "\nELEMENTS:\n";
    for (const auto& element : elements){
        std::cout << element << " ";
    }
    std::cout << std::endl << std::endl;
}

TEST(HashSetTest, Reserve) {
    tmn_associative::HashSet<int> hset;
    for (std::size_t i = 0; i < 10; ++i){
        hset.insert(50 + i);
    }
    hset.reserve(12);
    ASSERT_EQ(hset.buffer_size(), 32);
    hset.reserve(512);
    ASSERT_EQ(hset.buffer_size(), 512);
}