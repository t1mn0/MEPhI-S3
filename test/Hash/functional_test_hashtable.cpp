#include <gtest/gtest.h>

#include <cctype>

#include "../../include/Associative/HashTable.hpp"



TEST(HashTableTest, DefaultConstructor) {
    tmn::associative::HashTable<char, int> htable;
    ASSERT_EQ(htable.size(), 0);
    ASSERT_EQ(htable.buffer_size(), 256);
    ASSERT_EQ(htable.load_factor(), 0);
    ASSERT_EQ(htable.max_load_factor(), 0.5);
}

TEST(HashTableTest, CopyConstructor) {
    tmn::associative::HashTable<char, int> htable1;

    for (std::size_t i = 0; i < 26; ++i){
        tmn::Pair<const char, int> pair(97 + i, 97 + i); 
        htable1.insert(pair);
    }

    tmn::associative::HashTable<char, int> htable2(htable1);

    ASSERT_EQ(htable1.size(), 26);
    ASSERT_EQ(htable2.size(), 26);
    ASSERT_EQ(htable1.buffer_size(), 256);
    ASSERT_EQ(htable2.buffer_size(), 256);
    
    for (const auto& pair: htable1){
        ASSERT_EQ(pair.second, htable2[pair.first]);
    }
}

TEST(HashTableTest, MoveConstructor) {
    tmn::associative::HashTable<char, int> htable1;

    for (std::size_t i = 0; i < 26; ++i){
        tmn::Pair<const char, int> pair(97 + i, 97 + i); 
        htable1.insert(pair);
    }

    tmn::associative::HashTable<char, int> htable2(std::move(htable1));

    ASSERT_EQ(htable1.size(), 0);
    ASSERT_EQ(htable1.buffer_size(), 0);
    ASSERT_EQ(htable2.size(), 26);
    ASSERT_EQ(htable2.buffer_size(), 256);
}

TEST(HashTableTest, InitializerListConstructor) {
    tmn::associative::HashTable<char, float> htable = {{'a', 78.2}, {'f', 3.1415}, {'Q', 2.43}, {'W', 2.45}, {'Z', 0.41}};

    ASSERT_EQ(htable.size(), 5);
    ASSERT_EQ(htable.buffer_size(), 256);
}

TEST(HashTableTest, Swap) {
    tmn::associative::HashTable<char, int> htable1;
    tmn::associative::HashTable<char, int> htable2;

    for (std::size_t i = 0; i < 26; ++i){
        htable1.insert({char(97 + i), int(97 + i)});
    }

    for (std::size_t i = 0; i < 13; ++i){
        htable2.insert({char(65 + i), int(65 + i)});
    } 

    htable1.swap(htable2);

    ASSERT_EQ(htable1.size(), 13);
    ASSERT_EQ(htable2.size(), 26);

    for (const auto& pair: htable1){
        ASSERT_FALSE(std::islower(pair.first));
    }

    for (const auto& pair: htable2){
        ASSERT_TRUE(std::islower(pair.first));
    }
}

TEST(HashTableTest, CopyAssignment) {
    tmn::associative::HashTable<char, int> htable1;
    tmn::associative::HashTable<char, int> htable2;

    for (std::size_t i = 0; i < 26; ++i){
        htable1.insert({char(97 + i), int(97 + i)});
    }

    for (std::size_t i = 0; i < 13; ++i){
        htable2.insert({char(65 + i), int(65 + i)});
    } 

    htable1 = htable2;

    ASSERT_EQ(htable1.size(), 13);
    ASSERT_EQ(htable2.size(), 13);

    for (const auto& pair: htable1){
        ASSERT_EQ(pair.second, htable2[pair.first]);
    }
}

TEST(HashTableTest, MoveAssignment) {
    tmn::associative::HashTable<char, int> htable1;
    tmn::associative::HashTable<char, int> htable2;

    for (std::size_t i = 0; i < 26; ++i){
        htable1.insert({char(97 + i), int(97 + i)});
    }

    for (std::size_t i = 0; i < 13; ++i){
        htable2.insert({char(65 + i), int(65 + i)});
    } 

    htable1 = std::move(htable2);

    ASSERT_EQ(htable1.size(), 13);
    ASSERT_EQ(htable2.size(), 0);
}

TEST(HashTableTest, GetFields) {
    tmn::associative::HashTable<char, int> htable;

    for (std::size_t i = 0; i < 123; ++i){
        htable.insert({char(48 + i), int(48 + i)});
    }

    ASSERT_EQ(htable.size(), 123);
    ASSERT_FALSE(htable.empty());
    ASSERT_EQ(htable.buffer_size(), 256);
    ASSERT_EQ(htable.max_load_factor(), 0.5);

    std::cout << "\nCURRENT LOAD FACTOR: " << htable.load_factor() << "\n\n";
}

TEST(HashTableTest, Insert) {
    tmn::associative::HashTable<char, int> htable;
    for (std::size_t i = 0; i < 10; ++i){
        tmn::Pair<const char, int> pair(65 + i, i); 
        htable.insert(pair);
    }
    ASSERT_EQ(htable.size(), 10);
    ASSERT_EQ(htable.buffer_size(), 256);
    ASSERT_EQ(htable.load_factor(), 0.0390625);
}

TEST(HashTableTest, Insert624) {
    tmn::associative::HashTable<int, int> htable;
    for (std::size_t i = 0; i < 624; ++i){
        tmn::Pair<const int, int> pair(i, i * 100); 
        htable.insert(pair);
    }
    ASSERT_EQ(htable.size(), 624);
    ASSERT_EQ(htable.buffer_size(), 2048);
    // std::cout << "\nCURRENT BUFFER SIZE: " << htable.buffer_size() << "\n";
    // std::cout << "\nCURRENT LOAD FACTOR: " << htable.load_factor() << "\n\n";
}

TEST(HashTableTest, InsertRvalue) {
    tmn::associative::HashTable<int, int> htable;
    for (std::size_t i = 0; i < 111; ++i){
        tmn::Pair<const int, int> pair(i, i * 10); 
        htable.insert(std::move(pair));
    }
    ASSERT_EQ(htable.size(), 111);
    ASSERT_EQ(htable.buffer_size(), 256);
    std::cout << "\nCURRENT LOAD FACTOR: " << htable.load_factor() << "\n\n";
}

TEST(HashTableTest, EraseRvalue) {
    tmn::associative::HashTable<char, int> htable;
    for (std::size_t i = 0; i < 26; ++i){
        tmn::Pair<const char, int> pair(65 + i, 65 + i); 
        htable.insert(pair);
    }
    for (std::size_t i = 0; i < 10; ++i){
        ASSERT_TRUE(htable.erase(75 + i));
    }
    ASSERT_EQ(htable.size(), 16);
    ASSERT_FALSE(htable.erase(48));
    ASSERT_FALSE(htable.erase(80));
    ASSERT_FALSE(htable.erase(91));
    ASSERT_FALSE(htable.erase(64));
}

TEST(HashTableTest, Clear) {
    tmn::associative::HashTable<char, float> htable = {
        {'Q', 144}, 
        {'W', 0.169}, 
        {'E', 0.00025}, 
        {'R', -0.0001}, 
        {'T', 25.561},
        {'Y', -1.5}
    };

    ASSERT_EQ(htable.size(), 6);
    ASSERT_EQ(htable.buffer_size(), 256);
    htable.clear();
    ASSERT_EQ(htable.size(), 0);
    ASSERT_EQ(htable.buffer_size(), 256);
}

TEST(HashTableTest, ArrayOperator) {
    tmn::associative::HashTable<char, std::size_t> htable = {
        {'Q', 1}, 
        {'W', 4}, 
        {'E', 9}, 
        {'R', 25}, 
        {'T', 36},
        {'Y', 49}
    };

    std::size_t getQ = htable['Q'];
    getQ += 168;
    ASSERT_NE(getQ, htable['Q']);
    
    htable['G'] = 121;
    htable['X'] = 2025;
    
    // ASSERT_EQ(121, htable['G']);
    // ASSERT_EQ(2025, htable['X']);

    htable['W'] = 64;
    ASSERT_EQ(64, htable['W']);

    char t = 'T';
    ASSERT_EQ(36, htable[t]);
}

TEST(HashTableTest, GetByKey) {
    tmn::associative::HashTable<char, std::size_t> htable = {
        {'Q', 1}, 
        {'W', 4}, 
        {'E', 9}, 
        {'R', 16}, 
        {'T', 25},
        {'Y', 36}
    };

    ASSERT_TRUE(htable.get('Q').has_value());
    std::size_t getQ = htable.get('Q').value();
    ASSERT_EQ(getQ, htable['Q']);
    
    ASSERT_FALSE(htable.get('G').has_value());
}

TEST(HashTableTest, Contains) {
    tmn::associative::HashTable<char, int> htable;

    for (std::size_t i = 0; i < 26; ++i){
        tmn::Pair<const char, int> pair(97 + i, 97 + i); 
        htable.insert(pair);
    }
    
    ASSERT_FALSE(htable.contains('A'));
    ASSERT_FALSE(htable.contains('@'));
    ASSERT_FALSE(htable.contains('~'));
    ASSERT_FALSE(htable.contains('}'));
    ASSERT_FALSE(htable.contains('Y'));
}

TEST(HashTableTest, Keys) {
    tmn::associative::HashTable<char, int> htable;

    for (std::size_t i = 0; i < 26; ++i){
        tmn::Pair<const char, int> pair(97 + i, 97 + i); 
        htable.insert(pair);
    }
    
    tmn::sequence::ArraySequence<char> keys = htable.keys();
    ASSERT_EQ(keys.size(), 26);
    
    std::cout << "\nKEYS:\n";
    for (const auto& key : keys){
        std::cout << key << " ";
    }
    std::cout << std::endl << std::endl;
}

TEST(HashTableTest, Values) {
    tmn::associative::HashTable<char, int> htable;

    for (std::size_t i = 0; i < 26; ++i){
        tmn::Pair<const char, int> pair(97 + i, (97 + i) * 10); 
        htable.insert(pair);
    }
    
    tmn::sequence::ArraySequence<int> values = htable.values();
    ASSERT_EQ(values.size(), 26);

    std::cout << "\nVALUES:\n";
    for (const auto& value : values){
        std::cout << value << " ";
    }
    std::cout << std::endl << std::endl;
}

TEST(HashTableTest, Reserve) {
    tmn::associative::HashTable<char, int> htable;
    for (std::size_t i = 0; i < 10; ++i){
        tmn::Pair<const char, int> pair(48 + i, i); 
        htable.insert(pair);
    }
    htable.reserve(12);
    ASSERT_EQ(htable.buffer_size(), 256);
    htable.reserve(512);
    ASSERT_EQ(htable.buffer_size(), 512);
}