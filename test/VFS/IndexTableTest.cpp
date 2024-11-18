#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>
#include <string>
#include <algorithm>

#include "../../include/Associative/HashTable.hpp"
#include "../../include/VFS/FileDescriptor.hpp"

std::vector<std::string> LoadStringsFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::vector<std::string> strings;
    std::string line;
    while (std::getline(file, line)) {
        strings.push_back(line);
    }
    file.close();
    if (strings.empty()) {
        throw std::runtime_error("File is empty: " + filename);
    }
    return strings;
}

std::string RandomTime() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib_h(0, 23);
    std::uniform_int_distribution<> distrib_m(0, 59);
    std::uniform_int_distribution<> distrib_d(1, 30);
    std::uniform_int_distribution<> distrib_M(1, 12);
    std::uniform_int_distribution<> distrib_y(25, 30);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << distrib_h(gen) << ":"
       << std::setfill('0') << std::setw(2) << distrib_m(gen) << " "
       << std::setfill('0') << std::setw(2) << distrib_d(gen) << "."
       << std::setfill('0') << std::setw(2) << distrib_M(gen) << "."
       << std::setfill('0') << std::setw(2) << distrib_y(gen);
    return ss.str();
}

// without extension
std::string RandomFilePath(const std::vector<std::string>& pool) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> num_dirs(0, 5);
    std::uniform_int_distribution<> distrib_filenames(0, pool.size() - 1);
    std::uniform_int_distribution<> num_permissions(0, 3);

    std::stringstream path;
    path << "FS/";
    int num = num_dirs(gen);
    for (int j = 0; j < num; ++j) {
        path << pool[distrib_filenames(gen)] << "/";
    }
    path << pool[distrib_filenames(gen)];
    return path.str();
}


TEST(IndexTableTest, SearchTest) {
    // For now, the that paths is absolute - it's all temporary
    auto usernames = LoadStringsFromFile("/home/timno/Documents/MEPhI-S3/test/VFS/_username.txt");
    auto filenames = LoadStringsFromFile("/home/timno/Documents/MEPhI-S3/test/VFS/_filename.txt");

    std::ofstream record_file("/home/timno/Documents/MEPhI-S3/test/_test_results/fs_indextable.txt");
    std::ofstream search_file("/home/timno/Documents/MEPhI-S3/test/_test_results/search_test_indextable.txt");
    if (!record_file.is_open()) {
        FAIL() << "Could not open output file";
    }
    if (!search_file.is_open()) {
        FAIL() << "Could not open output file";
    }

    auto start = std::chrono::high_resolution_clock::now();
    tmn_associative::HashTable<std::string, tmn_vfs::FileDescriptor> index_table;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib_usernames(0, usernames.size() - 1);
    std::uniform_int_distribution<> distrib_filenames(0, filenames.size() - 1);
    std::uniform_int_distribution<> is_dir_distrib(0, 1);
    std::uniform_int_distribution<> num_inner_files(0, 8);

    for (int i = 0; i < 100000; ++i) {
        std::string physical_path = RandomFilePath(filenames);;
        std::string virtual_path = RandomFilePath(filenames);
        std::string owner_name = usernames[distrib_usernames(gen)];
        std::string modification_time = RandomTime();
        bool is_dir = is_dir_distrib(gen);
        if (!is_dir){
            virtual_path += ".txt";
        }
        
        tmn_vfs::FileDescriptor fd(is_dir, physical_path, virtual_path, owner_name);

        if (is_dir) {
            int inner_files_count = num_inner_files(gen);
            if (inner_files_count != 0){
                virtual_path += "/";
                record_file << i + 1 << ") " << virtual_path << " : " << inner_files_count << " inner files:" << std::endl;
            }
            for (int k = 0; k < inner_files_count; ++k) {
                std::string inner_filename = "inner_";
                inner_filename += std::to_string(k);
                inner_filename += ".md";
                fd.AddInnerFile(inner_filename);
                tmn_vfs::FileDescriptor inner_fd(false, physical_path, virtual_path + inner_filename, owner_name);
                index_table.insert({virtual_path + inner_filename, inner_fd});
                record_file << "  - " << inner_filename << std::endl;
            }
        }

        index_table.insert({virtual_path, fd});
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Elapsed time: " << elapsed.count() << " ms" << std::endl;
    record_file.close();

// ! - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    auto filenames_keys = index_table.keys();
    for (auto& filename : filenames_keys){
        auto search_start = std::chrono::high_resolution_clock::now();
        bool found = index_table.contains(filename);
        auto search_end = std::chrono::high_resolution_clock::now();
        auto search_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(search_end - search_start);
        search_file << "Search for '" << filename << "' took " << search_elapsed.count() << " nanoseconds" << std::endl;
    }
}
