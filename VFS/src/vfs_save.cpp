#include <string>
#include <fstream> 
#include <filesystem>

#include "../include/VFS.hpp"
#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn_vfs {

void VirtualFileSystem::SaveUsers() const {
    std::ofstream users_file(USERS_PATH);

    if (!users_file.is_open()) {
        std::string err_message = "Error(SaveUsers): error with opening file";
        throw tmn_exception::RuntimeException(err_message);
    }

    for (const auto& pair : users_table) {
        users_file << pair.second.toString() << std::endl;
    }
}

void VirtualFileSystem::SaveGroups() const {
    std::ofstream groups_file(GROUPS_PATH);

    if (!groups_file.is_open()) {
        std::string err_message = "Error(SaveGroups): error with opening file";
        throw tmn_exception::RuntimeException(err_message);
    }

    for (const auto& pair : groups_table) {
        groups_file << pair.second.toString() << std::endl;
    }
}

void VirtualFileSystem::SaveDescriptors() const {
    std::ofstream descriptors_file(FILE_DESCRIPTORS_PATH);

    if (!descriptors_file.is_open()) {
        std::string err_message = "Error(SaveDescriptors): error with opening file";
        throw tmn_exception::RuntimeException(err_message);
    }

    for (const auto& pair : files) {
        descriptors_file << pair.second.toString() << std::endl;
    }
}

void VirtualFileSystem::SaveConfig() const {
    std::ofstream config_file(VFS_CONFIG_PATH);

    if (!config_file.is_open()) {
        std::string err_message = "Error(SaveSystemParameters): error with opening file";
        throw tmn_exception::RuntimeException(err_message);
    }

    for (const auto& path : recording_files) {
        config_file << path.second << std::endl;
    }
}

void VirtualFileSystem::SaveAll() const {
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