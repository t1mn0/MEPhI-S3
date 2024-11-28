#include <string>
#include <fstream> 
#include <filesystem>
#include <iostream> // for std::cerr

#include "../include/VFS.hpp"
#include "../include/vfs_constants.hpp"

namespace tmn_vfs {

void VirtualFileSystem::SaveUsers() const noexcept {
    std::ofstream users_file(USERS_PATH);

    if (!users_file.is_open()) {
        std::cerr << "Error(SaveUsers): error with opening file: " << USERS_PATH << std::endl;
        return;
    }

    for (const auto& pair : users_table) {
        users_file << pair.second.toString() << std::endl;
    }
}

void VirtualFileSystem::SaveGroups() const noexcept {
    std::ofstream groups_file(GROUPS_PATH);

    if (!groups_file.is_open()) {
        std::cerr << "Error(SaveGroups): error with opening file: " << GROUPS_PATH << std::endl;
        return;
    }

    for (const auto& pair : groups_table) {
        groups_file << pair.second.toString() << std::endl;
    }
}

void VirtualFileSystem::SaveDescriptors() const noexcept {
    std::ofstream descriptors_file(FILE_DESCRIPTORS_PATH);

    if (!descriptors_file.is_open()) {
        std::cerr << "Error(SaveDescriptors): error with opening file: " << FILE_DESCRIPTORS_PATH << std::endl;
        return;
    }

    for (const auto& pair : files) {
        descriptors_file << pair.second.toString() << std::endl;
    }
}

void VirtualFileSystem::SaveConfig() const noexcept {
    std::ofstream config_file(VFS_CONFIG_PATH);

    if (!config_file.is_open()) {
        std::cerr << "Error(SaveSystemParameters): error with opening file: " << VFS_CONFIG_PATH << std::endl; 
        return;
    }

    for (const auto& path : recording_files) {
        config_file << path.second << std::endl;
    }
}

void VirtualFileSystem::SaveAll() const noexcept {
    SaveUsers();
    SaveGroups();
    SaveDescriptors();
    SaveConfig();
}

VirtualFileSystem::~VirtualFileSystem(){
    SaveAll();
    usernames.clear();
    groupnames.clear();
    recording_files.clear();
    files.clear();
    users_table.clear();
    groups_table.clear();
}

}