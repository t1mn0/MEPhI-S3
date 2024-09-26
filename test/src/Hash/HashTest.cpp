#include <iostream>
#include <random>
#include <unordered_set>
#include <map>
#include <limits>

#include "../../include/Associative/Hash.hpp"


void hash_string_test() {
    std::unordered_set<std::string> test_strings;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(65, 90);

    for(int i = 0; i < 1000; ++i){
        std::string random_string;
        for(int j = 0; j < 20; ++j){
            random_string += static_cast<char>(distrib(gen));
        }
        test_strings.insert(random_string);
    }

    std::map<std::size_t, int> hash_collisions;

    for (const auto& str : test_strings) {
        std::size_t hash_val = tmn_hash::Hash(str);
        ++hash_collisions[hash_val];
    }

    int num_collisions = 0;
    for (const auto& pair : hash_collisions) {
        if (pair.second > 1) {
            num_collisions++;
        }
    }

    std::cout << "\nTotal number of collisions: " << num_collisions << std::endl;
    std::cout << "Total number of strings: " << test_strings.size() << std::endl;
}

void hash_test_int() {
    std::unordered_set<int> test_values;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

    for (int i = 0; i < 1000; ++i) {
        test_values.insert(distrib(gen));
    }

    //int i = 0;
    std::map<size_t, int> hash_collisions;
    for (const auto& val : test_values) {
        size_t hash_val = tmn_hash::Hash(val);
        ++hash_collisions[hash_val];
        
        // std::cout << i <<") meow!~ " << hash_val << std::endl;
        // ++i;
    }

    int num_collisions = 0;
    for (const auto& pair : hash_collisions) {
        if (pair.second > 1) {
            num_collisions++;
        }
    }

    std::cout << "\nTotal number of collisions: " << num_collisions << std::endl;
    std::cout << "Total number of values: " << test_values.size() << std::endl;
}

void hash_test_long() {
    std::unordered_set<long> test_values;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<long> distrib(std::numeric_limits<long>::min(), std::numeric_limits<long>::max());

    for (int i = 0; i < 1000; ++i) {
        test_values.insert(distrib(gen));
    }

    std::map<size_t, int> hash_collisions;
    for (const auto& val : test_values) {
        size_t hash_val = tmn_hash::Hash(val);
        ++hash_collisions[hash_val];
    }

    int num_collisions = 0;
    for (const auto& pair : hash_collisions) {
        if (pair.second > 1) {
            num_collisions++;
        }
    }

    std::cout << "\nTotal number of collisions: " << num_collisions << std::endl;
    std::cout << "Total number of values: " << test_values.size() << std::endl;
}


void hash_test_ulong() {
    std::unordered_set<unsigned long> test_values;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned long> distrib(0, std::numeric_limits<unsigned long>::max());

    for (int i = 0; i < 1000; ++i) {
        test_values.insert(distrib(gen));
    }

    std::map<size_t, int> hash_collisions;
    for (const auto& val : test_values) {
        size_t hash_val = tmn_hash::Hash(val);
        ++hash_collisions[hash_val];
    }

    int num_collisions = 0;
    for (const auto& pair : hash_collisions) {
        if (pair.second > 1) {
            num_collisions++;
        }
    }

    std::cout << "\nTotal number of collisions: " << num_collisions << std::endl;
    std::cout << "Total number of values: " << test_values.size() << std::endl;
}


void hash_test_double() {
    std::unordered_set<double> test_values;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> distrib(-1000.0, 1000.0);

    for (int i = 0; i < 1000; ++i) {
        test_values.insert(distrib(gen));
    }


    std::map<size_t, int> hash_collisions;
    for (const auto& val : test_values) {
        size_t hash_val = tmn_hash::Hash(val);
        ++hash_collisions[hash_val];
    }

    int num_collisions = 0;
    for (const auto& pair : hash_collisions) {
        if (pair.second > 1) {
            num_collisions++;
        }
    }

    std::cout << "\nTotal number of collisions: " << num_collisions << std::endl;
    std::cout << "Total number of values: " << test_values.size() << std::endl;
}

void hash_test_short() {
    std::unordered_set<short> test_values;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<short> distrib(std::numeric_limits<short>::min(), std::numeric_limits<short>::max());

    for (int i = 0; i < 500; ++i) {
        test_values.insert(distrib(gen));
    }

    std::map<size_t, int> hash_collisions;
    for (const auto& val : test_values) {
        size_t hash_val = tmn_hash::Hash(val);
        ++hash_collisions[hash_val];
    }

    int num_collisions = 0;
    for (const auto& pair : hash_collisions) {
        if (pair.second > 1) {
            num_collisions++;
        }
    }

    std::cout << "\nTotal number of collisions: " << num_collisions << std::endl;
    std::cout << "Total number of values: " << test_values.size() << std::endl;
}


void hash_test_char() {
    std::unordered_set<char> test_values;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<char> distrib(33, 126);

    for (int i = 0; i < 500; ++i) {
        test_values.insert(distrib(gen));
    }

    std::map<size_t, int> hash_collisions;
    for (const auto& val : test_values) {
        size_t hash_val = tmn_hash::Hash(val);
        ++hash_collisions[hash_val];
    }

    int num_collisions = 0;
    for (const auto& pair : hash_collisions) {
        if (pair.second > 1) {
            num_collisions++;
        }
    }

    std::cout << "\nTotal number of collisions: " << num_collisions << std::endl;
    std::cout << "Total number of values: " << test_values.size() << std::endl;
}


void hash_test_float() {
    std::unordered_set<float> test_values;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distrib(-1000.0, 1000.0);

    for (int i = 0; i < 1000; ++i) {
        test_values.insert(distrib(gen));
    }

    std::map<size_t, int> hash_collisions;
    for (const auto& val : test_values) {
        size_t hash_val = tmn_hash::Hash(val);
        ++hash_collisions[hash_val];
    }

    int num_collisions = 0;
    for (const auto& pair : hash_collisions) {
        if (pair.second > 1) {
            num_collisions++;
        }
    }

    std::cout << "\nTotal number of collisions: " << num_collisions << std::endl;
    std::cout << "Total number of values: " << test_values.size() << std::endl;
}