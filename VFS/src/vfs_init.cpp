#include <string>
#include <fstream> 
#include <filesystem>

#include "../include/VFS.hpp"
#include "../include/Utils.hpp" // for GetTimeNow()
#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn::vfs {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Check the validity of the existing virtual file system :

bool VirtualFileSystem::in_system(){
    std::filesystem::path entry_point(ENTRY_POINT_PATH);

    return (is_valid_system_files(entry_point / USERS_PATH) && 
        is_valid_system_files(entry_point / GROUPS_PATH) && 
        is_valid_system_files(entry_point / FILE_DESCRIPTORS_PATH) &&
        is_valid_system_files(entry_point / VFS_CONFIG_PATH));
}

bool VirtualFileSystem::is_valid_system_files(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)){
        return false;
    }

    std::ifstream file(path);
    if (!file.is_open()) return false;

    return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Configuration for initialization for the first time :

void VirtualFileSystem::create_helper_files() {
    std::filesystem::path entry_point(ENTRY_POINT_PATH);

    if (std::filesystem::exists(entry_point / USERS_PATH)) std::filesystem::remove(entry_point / USERS_PATH);
    if (std::filesystem::exists(entry_point / GROUPS_PATH)) std::filesystem::remove(entry_point / GROUPS_PATH);
    if (std::filesystem::exists(entry_point / FILE_DESCRIPTORS_PATH)) std::filesystem::remove(entry_point / FILE_DESCRIPTORS_PATH);
    if (std::filesystem::exists(entry_point / VFS_CONFIG_PATH)) std::filesystem::remove(entry_point / VFS_CONFIG_PATH);

    std::ofstream users_file(entry_point / USERS_PATH);
    std::ofstream groups_file(entry_point / GROUPS_PATH);
    std::ofstream descriptors_file(entry_point / FILE_DESCRIPTORS_PATH);
    std::ofstream config_file(entry_point / VFS_CONFIG_PATH);

    if (!users_file.is_open() || !groups_file.is_open() || !descriptors_file.is_open() || !config_file.is_open()) {
        std::string err_message = "Error(create_helper_files): Could not create helper files";
        throw tmn::exception::RuntimeException(err_message);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Initialization of the table structures that make up the VFS :

void VirtualFileSystem::init_users() {
    std::ifstream users_file(USERS_PATH);
    if (!users_file.is_open()) {
        std::string err_message = "Error(init_users): Could not open users file";
        throw tmn::exception::RuntimeException(err_message);
    }

    std::string line;
    while (std::getline(users_file, line) && !line.empty()) {
        User user = User::from_string(line);
        users_table.insert({user.user_id, user});
        usernames.insert({user.username, user.user_id});
    }
}

void VirtualFileSystem::init_groups() {
    std::ifstream groups_file(GROUPS_PATH);
    if (!groups_file.is_open()) {
        std::string err_message = "Error(init_groups): Could not open groups file";
        throw tmn::exception::RuntimeException(err_message);
    }

    std::string line;
    while (std::getline(groups_file, line) && !line.empty()) {
        Group group = Group::from_string(line);
        groups_table.insert({group.group_id, group});
        groupnames.insert({group.groupname, group.group_id});
    }
}

void VirtualFileSystem::init_file_descriptors() {
    std::ifstream descriptors_file(FILE_DESCRIPTORS_PATH);
    if (!descriptors_file.is_open()) {
        std::string err_message = "Error(init_file_descriptors): Could not open descriptors file";
        throw tmn::exception::RuntimeException(err_message);
    }

    std::string line;
    while (std::getline(descriptors_file, line) && !line.empty()) {
        FileDescriptor fd = FileDescriptor::from_string(line);
        files.insert({fd.fd_id, fd});
    }
}

void VirtualFileSystem::init_system_parameters(){
    std::ifstream config_file(VFS_CONFIG_PATH);
    if (!config_file.is_open()) {
        std::string err_message = "Error(init_system_parameters): Could not open config file";
        throw tmn::exception::RuntimeException(err_message);
    }

    std::string line;
    std::size_t i = 0;
    while (std::getline(config_file, line) && !line.empty()) {
        recording_files.insert({i + 1, line});
        ++i;
    }
}

VirtualFileSystem VirtualFileSystem::init(std::string root_password) {
    VirtualFileSystem vfs;
    if (in_system()){ // checks the integrity of the file system
        vfs.init_users();
        vfs.init_groups();
        vfs.init_file_descriptors();
        vfs.init_system_parameters();
        vfs.active_user = 0;
        vfs.current_directory = 0;
        vfs.fd_id = vfs.files.size();
        vfs.user_id = vfs.users_table.size();
        vfs.group_id = vfs.groups_table.size();
        vfs.rec_id = vfs.recording_files.size();
    }
    else{
        create_helper_files();
        User user(0, SUPER_USERNAME, SUPER_FULLNAME, get_time_now(), std::to_string(std::hash<std::string>{}(root_password)), UserStatus::SUPER);
        Group default_group(user.user_id, user.username, user.user_id, user.creation_time);
        FileDescriptor fd(0, true, 0, 0, "/", 0, 0, 0, FilePermissions(Permission::READWRITE, Permission::READWRITE, Permission::READWRITE));
        
        vfs.active_user = 0;

        user.groups.insert(user.user_id);
        vfs.users_table.insert(tmn::Pair<const uint64_t, User>(user.user_id, user));
        vfs.usernames.insert(tmn::Pair<const std::string, uint64_t>(user.username, user.user_id));
        
        default_group.members.insert(user.user_id);

        vfs.groups_table.insert(tmn::Pair<const uint64_t, Group>(default_group.group_id, default_group));
        vfs.groupnames.insert(tmn::Pair<const std::string, uint64_t>(default_group.groupname, default_group.group_id));

        vfs.files.insert({fd.fd_id, fd});
        vfs.files[fd.fd_id].inner_files.insert(0);
    }

    return vfs;
}

}