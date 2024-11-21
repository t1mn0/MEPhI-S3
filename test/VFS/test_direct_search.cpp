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

std::vector<std::string> LoadStringsFromFile(const std::string& filename);

std::string RandomTime();

// without extension
std::string RandomFilePath(const std::vector<std::string>& pool);


TEST(IndexTableTest, DirectSearchTest) {
// ! INIT INDEX TABLE - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // For now, the that paths is absolute - it's all temporary
    auto usernames = LoadStringsFromFile("/home/timno/Documents/MEPhI-S3/test/VFS/_username.txt");
    auto filenames = LoadStringsFromFile("/home/timno/Documents/MEPhI-S3/test/VFS/_filename.txt");

    std::ofstream recordFS_file("/home/timno/Documents/MEPhI-S3/test/_test_results/fs_indextable.txt");
    std::ofstream direct_search_results_file("/home/timno/Documents/MEPhI-S3/test/_test_results/direct_search_test_indextable.txt");
    if (!recordFS_file.is_open()) {
        FAIL() << "Could not open output file";
    }
    if (!direct_search_results_file.is_open()) {
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

    for (int i = 0; i < 10000; ++i) {
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
                recordFS_file << i + 1 << ") " << virtual_path << " : " << inner_files_count << " inner files:" << std::endl;
            }
            for (int k = 0; k < inner_files_count; ++k) {
                std::string inner_filename = filenames[distrib_filenames(gen)];
                inner_filename += ".md";
                fd.AddInnerFile(inner_filename);
                tmn_vfs::FileDescriptor inner_fd(false, physical_path, virtual_path + inner_filename, owner_name);
                index_table.insert({virtual_path + inner_filename, inner_fd});
                recordFS_file << "    - " << inner_filename << std::endl;
            }
        }
        else{
            recordFS_file << i + 1 << ") " << virtual_path << " :  0 inner files;" << std::endl;
        }

        index_table.insert({virtual_path, fd});
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    
    std::cout << "Elapsed time: " << elapsed.count() << " seconds" << std::endl;
    recordFS_file.close();

// ! DIRECT SEARCHING - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    auto filepaths_keys = index_table.keys();
    for (auto& filepath : filepaths_keys){
        auto search_start = std::chrono::high_resolution_clock::now();
        bool found = index_table.contains(filepath);
        auto search_end = std::chrono::high_resolution_clock::now();
        auto search_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(search_end - search_start);
        direct_search_results_file << "Search FileDescriptor for '" << filepath << "' took " << search_elapsed.count() << " nanoseconds" << std::endl;
    }
}

