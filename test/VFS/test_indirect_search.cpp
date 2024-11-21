#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>
#include <string>

#include "../../include/Associative/HashTable.hpp"
#include "../../include/VFS/FileDescriptor.hpp"

// TODO : link one realization for two load test (dir/indir)

std::vector<std::string> LoadStringsFromFile(const std::string& filepath);

std::string RandomTime();

// without extension
std::string RandomFilePath(const std::vector<std::string>& pool);

std::string CutFileName(const std::string& path);

TEST(IndexTableTest, IndirectSearchTest) {
// ! INIT INDEX TABLE - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // For now, the that paths is absolute - it's all temporary
    auto usernames = LoadStringsFromFile("/home/timno/Documents/MEPhI-S3/test/VFS/_username.txt");
    auto filenames = LoadStringsFromFile("/home/timno/Documents/MEPhI-S3/test/VFS/_filename.txt");

    std::ofstream recordFS_file("/home/timno/Documents/MEPhI-S3/test/_test_results/fs_indextable.txt");
    std::ofstream indirect_search_results_file("/home/timno/Documents/MEPhI-S3/test/_test_results/indirect_search_test_indextable.txt");
    if (!recordFS_file.is_open()) {
        FAIL() << "Could not open output file";
    }
    if (!indirect_search_results_file.is_open()) {
        FAIL() << "Could not open output file";
    }

    auto start = std::chrono::high_resolution_clock::now();
    tmn_associative::HashTable<std::string, tmn_vfs::FileDescriptor> index_table;
    tmn_associative::HashTable<std::string, tmn_associative::HashSet<std::string>> indirect_index_table;

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

        std::string virtual_file_name = CutFileName(virtual_path);

        if (indirect_index_table.contains(virtual_file_name)){
            indirect_index_table[virtual_file_name].insert(virtual_path);
        }
        else{
            tmn_associative::HashSet<std::string> possible_paths;
            possible_paths.insert(virtual_path);
            indirect_index_table.insert({virtual_file_name, possible_paths});
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
                indirect_index_table[inner_filename].insert(virtual_path + inner_filename);
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

// ! INDIRECT SEARCHING - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

    auto filenames_keys = indirect_index_table.keys();
    for (auto& filename : filenames_keys){
        indirect_search_results_file << "Searching file by name '" << filename << "'..." << std::endl;
        auto search_start = std::chrono::high_resolution_clock::now();
        auto& associative_paths = indirect_index_table[filename];
        indirect_search_results_file << "  Found " << associative_paths.size() <<  " possible paths:"<< std::endl;
        for (auto& path : associative_paths){
            bool found = index_table.contains(path);
            indirect_search_results_file << "    - path :" << path <<  ";" << std::endl;
        }
        auto search_end = std::chrono::high_resolution_clock::now();
        auto search_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(search_end - search_start);
        indirect_search_results_file << "Search all possible FileDescriptors for '" << filename << "' took " << search_elapsed.count() << " microseconds" << std::endl;
    }
}
