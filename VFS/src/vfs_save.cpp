#include <string>
#include <fstream> 
#include <filesystem>

#include "../include/VFS.hpp"
#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn::vfs {

void VirtualFileSystem::save_users() const {
    std::ofstream users_file(USERS_PATH);

    if (!users_file.is_open()) {
        std::string err_message = "Error(save_users): error with opening file";
        throw tmn::exception::RuntimeException(err_message);
    }

    for (const auto& pair : users_table) {
        users_file << pair.second.to_string() << std::endl;
    }
}

void VirtualFileSystem::save_groups() const {
    std::ofstream groups_file(GROUPS_PATH);

    if (!groups_file.is_open()) {
        std::string err_message = "Error(save_groups): error with opening file";
        throw tmn::exception::RuntimeException(err_message);
    }

    for (const auto& pair : groups_table) {
        groups_file << pair.second.to_string() << std::endl;
    }
}

void VirtualFileSystem::save_file_descriptors() const {
    std::ofstream descriptors_file(FILE_DESCRIPTORS_PATH);

    if (!descriptors_file.is_open()) {
        std::string err_message = "Error(save_file_descriptors): error with opening file";
        throw tmn::exception::RuntimeException(err_message);
    }

    for (const auto& pair : files) {
        descriptors_file << pair.second.to_string() << std::endl;
    }
}

void VirtualFileSystem::save_config() const {
    std::ofstream config_file(VFS_CONFIG_PATH);

    if (!config_file.is_open()) {
        std::string err_message = "Error(save_config): error with opening file";
        throw tmn::exception::RuntimeException(err_message);
    }

    for (const auto& path : recording_files) {
        config_file << path.second << std::endl;
    }
}

void VirtualFileSystem::save_all() const {
    save_users();
    save_groups();
    save_file_descriptors();
    save_config();
}

VirtualFileSystem::~VirtualFileSystem(){
    save_all();
    usernames.clear();
    groupnames.clear();
    recording_files.clear();
    files.clear();
    users_table.clear();
    groups_table.clear();
}

}