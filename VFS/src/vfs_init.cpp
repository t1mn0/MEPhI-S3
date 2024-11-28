#include <string>
#include <fstream> 
#include <filesystem>
#include <iostream> // for std::cerr

#include "../include/VFS.hpp"
#include "../include/Utils.hpp"
#include "../include/vfs_constants.hpp"

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
// Configuration for initialization for the first time:

unsigned long VirtualFileSystem::FindRecordingFiles(const std::filesystem::path& path, unsigned long parent_id) noexcept {
    if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)) {
        std::cerr << "Error(FindRecordingFiles): Passed argument 'PATH' is not a valid directory." << std::endl;
        return 0;
    }

    std::string time = GetTimeNow();

    FileDescriptor fd;

    for (const auto& entry : std::filesystem::directory_iterator(path)) {
        if (std::filesystem::is_regular_file(entry)) {
            recording_files.insert({recording_files.size() + 1, entry.path().string()});
        }

        if (std::filesystem::is_directory(entry)){
            fd.fd_id = files.size() + 1;
            fd.is_dir = true;
            fd.filename = entry.path().filename().string();
            fd.parent_dir_id = parent_id;
            fd.creation_time = time;
            fd.modification_time = time;
            fd.descriptor_modification_time = time;
            fd.file_permissions = FilePermissions(Permission::READWRITE, Permission::READWRITE, Permission::READWRITE);
            tmn::Pair<const unsigned long, FileDescriptor> pair(fd.fd_id, fd);
            files.insert(pair);

            files[parent_id].inner_files.insert(fd.fd_id);
            
            if (!std::filesystem::is_empty(entry.path())){
                files[fd.fd_id].inner_files.insert(FindRecordingFiles(entry.path(), fd.fd_id));
            }
        }
        else{
            fd.fd_id = files.size() + 1;
            fd.is_dir = false;
            fd.parent_dir_id = parent_id;
            fd.filename = entry.path().filename();
            fd.content_size = entry.file_size();
            fd.creation_time = time;
            fd.modification_time = time;
            fd.descriptor_modification_time = time;
            fd.file_permissions = FilePermissions(Permission::READWRITE, Permission::READWRITE, Permission::READ);

            files[parent_id].inner_files.insert(fd.fd_id);

            tmn::Pair<const unsigned long, FileDescriptor> pair(fd.fd_id, fd);
            files.insert(pair);
        }        
    }

    return fd.fd_id;
}

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
        std::cerr << "Error: Could not create helper files." << std::endl;
        return;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Initialization of the table structures that make up the VFS :

void VirtualFileSystem::InitUsers() {
    std::ifstream users_file(USERS_PATH);
    if (!users_file.is_open()) {
        std::cerr << "Error(InitUsers): Could not open users file: " << USERS_PATH << std::endl;
        return;
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
        std::cerr << "Error(InitGroups): Could not open users file: " << GROUPS_PATH << std::endl;
        return;
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
        std::cerr << "Error(InitFileDescriptors): Could not open users file: " << FILE_DESCRIPTORS_PATH << std::endl;
        return;
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
        std::cerr << "Error(InitSystemParameters): Could not open users file: " << VFS_CONFIG_PATH << std::endl;
        return;
    }

    std::string line;
    std::size_t i = 0;
    while (std::getline(config_file, line) && !line.empty()) {
        recording_files.insert({i, line});
        ++i;
    }
}

VirtualFileSystem VirtualFileSystem::Init() {
    VirtualFileSystem vfs;
    if (VFSInSystem()){ // checks the integrity of the file system
        vfs.InitUsers();
        vfs.InitGroups();
        vfs.InitFileDescriptors();
        vfs.InitSystemParameters();
        vfs.active_user = 0;
    }
    else{
        CreateHelperFiles();
        FileDescriptor fd(0, true, 0, 0, "/", 0, 0, 0, FilePermissions(Permission::READWRITE, Permission::READWRITE, Permission::READWRITE));
        User user(0, SUPER_USERNAME, SUPER_FULLNAME, GetTimeNow(), "-", UserStatus::SUPER);
        vfs.active_user = 0;
        vfs.AddFile(fd);
        vfs.AddUser(user);
        vfs.FindRecordingFiles(std::filesystem::path(ENTRY_POINT_PATH), 0);
    }

    return vfs;
}

}