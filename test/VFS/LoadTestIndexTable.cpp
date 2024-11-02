#include <gtest/gtest.h>

#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <chrono>
#include <vector>
#include <string>

#include "../../include/Associative/HashTable.hpp"
#include "../../include/VFS/FileInfo.hpp"

std::string RandomUsername() {
    std::filesystem::path file_path = std::filesystem::current_path() / "_username.txt";

    std::ifstream file(file_path.string());

    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: _username.txt");
    }

    std::vector<std::string> usernames;
    std::string line;
    while (std::getline(file, line)) {
        usernames.push_back(line);
    }
    file.close();

    if (usernames.empty()) {
        throw std::runtime_error("File is empty: _username.txt");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, usernames.size() - 1);

    return usernames[distrib(gen)];
}

std::string RandomFilePath() {
    std::filesystem::path file_path = std::filesystem::current_path() / "_filename.txt";

    std::ifstream file(file_path.string());
    
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: _filename.txt");
    }

    std::vector<std::string> words;
    std::string line;
    while (std::getline(file, line)) {
        words.push_back(line);
    }
    file.close();

    if (words.empty()) {
        throw std::runtime_error("File is empty: _filename.txt");
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, words.size() - 1);
    std::uniform_int_distribution<> num_dirs(0, 6);

    std::stringstream path;
    path << "files/";
    int num = num_dirs(gen);
    for (int i = 0; i < num; ++i) {
        path << words[distrib(gen)] << "/";
    }
    path << words[distrib(gen)] << ".txt";

    return path.str();
}

std::string RandomTime() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib_h(0, 23);
    std::uniform_int_distribution<> distrib_m(0, 59);
    std::uniform_int_distribution<> distrib_s(0, 59);
    std::uniform_int_distribution<> distrib_d(1, 31);
    std::uniform_int_distribution<> distrib_M(1, 12);
    std::uniform_int_distribution<> distrib_y(2025, 2030);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << distrib_h(gen) << "-"
       << std::setfill('0') << std::setw(2) << distrib_m(gen) << "-"
       << std::setfill('0') << std::setw(2) << distrib_s(gen) << "-"
       << std::setfill('0') << std::setw(2) << distrib_d(gen) << "-"
       << std::setfill('0') << std::setw(2) << distrib_M(gen) << "-"
       << distrib_y(gen);
    return ss.str();
}



TEST(LoadTestIndexTable, LoadTestIndexTable){
    auto start = std::chrono::high_resolution_clock::now();
    tmn_associative::HashTable<std::string, tmn_vfs::FileInfo> index_table;

    for (int i = 0; i < 10; ++i){
        std::string physical_path = "";
        std::string virtual_path = RandomFilePath();
        std::string creator_name = RandomUsername();
        std::string modification_time = RandomTime();
        tmn_vfs::FileInfo fi(false, physical_path, virtual_path, creator_name);
        fi.SetModificationTime(modification_time);
        
        auto pair = *(new tmn::Pair<const std::string, tmn_vfs::FileInfo>(virtual_path, fi));
        index_table.insert(pair);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);    
}