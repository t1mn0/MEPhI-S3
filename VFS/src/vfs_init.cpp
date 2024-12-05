#include <string>
#include <fstream> 
#include <filesystem>

#include "../include/VFS.hpp"
#include "../include/Utils.hpp" // for GetTimeNow()
#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn_vfs {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Check the validity of the existing virtual file system :

bool VirtualFileSystem::VFSInSystem(){
    std::filesystem::path entry_point(ENTRY_POINT_PATH);

    return (IsValidSystemFile(entry_point / USERS_PATH) && 
        IsValidSystemFile(entry_point / GROUPS_PATH) && 
        IsValidSystemFile(entry_point / FILE_DESCRIPTORS_PATH) &&
        IsValidSystemFile(entry_point / VFS_CONFIG_PATH));
}

bool VirtualFileSystem::IsValidSystemFile(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)){
        return false;
    }

    std::ifstream file(path);
    if (!file.is_open()) return false;

    return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Configuration for initialization for the first time :

void VirtualFileSystem::CreateHelperFiles() {
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
        std::string err_message = "Error(CreateHelperFiles): Could not create helper files";
        throw tmn_exception::RuntimeException(err_message);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Initialization of the table structures that make up the VFS :

void VirtualFileSystem::InitUsers() {
    std::ifstream users_file(USERS_PATH);
    if (!users_file.is_open()) {
        std::string err_message = "Error(InitUsers): Could not open users file";
        throw tmn_exception::RuntimeException(err_message);
    }

    std::string line;
    while (std::getline(users_file, line) && !line.empty()) {
        User user = User::fromString(line);
        users_table.insert({user.user_id, user});
        usernames.insert({user.username, user.user_id});
    }
}

void VirtualFileSystem::InitGroups() {
    std::ifstream groups_file(GROUPS_PATH);
    if (!groups_file.is_open()) {
        std::string err_message = "Error(InitGroups): Could not open groups file";
        throw tmn_exception::RuntimeException(err_message);
    }

    std::string line;
    while (std::getline(groups_file, line) && !line.empty()) {
        Group group = Group::fromString(line);
        groups_table.insert({group.group_id, group});
        groupnames.insert({group.groupname, group.group_id});
    }
}

void VirtualFileSystem::InitFileDescriptors() {
    std::ifstream descriptors_file(FILE_DESCRIPTORS_PATH);
    if (!descriptors_file.is_open()) {
        std::string err_message = "Error(InitFileDescriptors): Could not open descriptors file";
        throw tmn_exception::RuntimeException(err_message);
    }

    std::string line;
    while (std::getline(descriptors_file, line) && !line.empty()) {
        FileDescriptor fd = FileDescriptor::fromString(line);
        files.insert({fd.fd_id, fd});
    }
}

void VirtualFileSystem::InitSystemParameters(){
    std::ifstream config_file(VFS_CONFIG_PATH);
    if (!config_file.is_open()) {
        std::string err_message = "Error(InitSystemParameters): Could not open config file";
        throw tmn_exception::RuntimeException(err_message);
    }

    std::string line;
    std::size_t i = 0;
    while (std::getline(config_file, line) && !line.empty()) {
        recording_files.insert({i + 1, line});
        ++i;
    }
}

VirtualFileSystem VirtualFileSystem::Init(std::string root_password) {
    VirtualFileSystem vfs;
    if (VFSInSystem()){ // checks the integrity of the file system
        vfs.InitUsers();
        vfs.InitGroups();
        vfs.InitFileDescriptors();
        vfs.InitSystemParameters();
        vfs.active_user = 0;
        vfs.current_directory = 0;
        vfs.fd_id = vfs.files.size();
        vfs.user_id = vfs.users_table.size();
        vfs.group_id = vfs.groups_table.size();
        vfs.rec_id = vfs.recording_files.size();
    }
    else{
        CreateHelperFiles();
        User user(0, SUPER_USERNAME, SUPER_FULLNAME, GetTimeNow(), std::to_string(std::hash<std::string>{}(root_password)), UserStatus::SUPER);
        Group default_group(user.user_id, user.username, user.user_id, user.creation_time);
        FileDescriptor fd(0, true, 0, 0, "/", 0, 0, 0, FilePermissions(Permission::READWRITE, Permission::READWRITE, Permission::READWRITE));
        
        vfs.active_user = 0;

        user.groups.insert(user.user_id);
        vfs.users_table.insert(tmn::Pair<const unsigned long, User>(user.user_id, user));
        vfs.usernames.insert(tmn::Pair<const std::string, unsigned long>(user.username, user.user_id));
        
        default_group.members.insert(user.user_id);

        vfs.groups_table.insert(tmn::Pair<const unsigned long, Group>(default_group.group_id, default_group));
        vfs.groupnames.insert(tmn::Pair<const std::string, unsigned long>(default_group.groupname, default_group.group_id));

        vfs.files.insert({fd.fd_id, fd});
        vfs.files[fd.fd_id].inner_files.insert(0);
    }

    return vfs;
}

}