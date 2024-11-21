#include <gtest/gtest.h>
#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include <vector>
#include <string>

std::vector<std::string> LoadStringsFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    std::vector<std::string> strings;
    std::string line;
    while (std::getline(file, line)) {
        strings.push_back(line);
    }
    file.close();
    if (strings.empty()) {
        throw std::runtime_error("File is empty: " + filepath);
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
    std::uniform_int_distribution<> distrib_filepaths(0, pool.size() - 1);
    std::uniform_int_distribution<> num_permissions(0, 3);

    std::stringstream path;
    path << "FS/";
    int num = num_dirs(gen);
    for (int j = 0; j < num; ++j) {
        path << pool[distrib_filepaths(gen)] << "/";
    }
    path << pool[distrib_filepaths(gen)];
    return path.str();
}

std::string CutFileName(const std::string& path) {
    size_t last_slash = path.find_last_of('/');
    if (last_slash == std::string::npos) {
        return path;
    }
    return path.substr(last_slash + 1);
}