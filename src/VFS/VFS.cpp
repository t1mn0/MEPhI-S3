#include "VFS.hpp"

#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>

namespace tmn_vfs{

uint16_t FILE_COUNT = 12;
std::string MOUNT_DIR = "../../examples";
std::string VFS_DIR = "VFS";



void VirtualFileSystem::MountVFS() {
    std::filesystem::path vfs_path(MOUNT_DIR + "/" + VFS_DIR);

    std::filesystem::create_directories(vfs_path / "files");

    for (int i = 0; i < FILE_COUNT; ++i) {
        std::ofstream file(vfs_path / "files" / ("File-" + std::to_string(i) + ".txt"));
        file.close();
    }
}

const std::string& VirtualFileSystem::WhoUser() const {
    return active_user;
}

const std::string tmn_vfs::VirtualFileSystem::AddUser(const User& user) {
    if(users_table.contains(user.GetUserName())){
        return "ERROR";
    }
    else{
        users_table.emplace(user.GetUserName(), user);
        return user.GetUserName();
    }
}

void VirtualFileSystem::Authorization(const std::string& username, const std::string& password_hash){
    if(!users_table.contains(username)){
        std::cerr << "Error: User not found: " << username << std::endl;
        return;
    }
    if(users_table[username].GetPasswordHash() == password_hash){
        active_user = username;
    }
}

bool VirtualFileSystem::IsUserInSystem(const std::string& username) {
    return users_table.contains(username);
}

void VirtualFileSystem::RemoveUser(const std::string& username){
    if(!users_table.contains(username)){
        std::cerr << "Error: User not found: " << username << std::endl;
        return;
    }
    for (const auto& groupname : users_table[username].groups){
        groups_table[groupname].members.erase(username);
    }   
    users_table.erase(username);
}

const std::string VirtualFileSystem::AddGroup(const Group& group){
    if(groups_table.contains(group.GetGroupName())){
        return "ERROR";
    }
    else{
        groups_table.insert({group.GetGroupName(), group});
        return group.GetGroupName();
    }
}

void VirtualFileSystem::AddToGroup(const std::string& username, const std::string& groupname, const std::string& password_to_group){
    if(!groups_table.contains(groupname)){
        std::cerr << "Error: Group not found: " << groupname << std::endl;
        return;
    }
    if(!users_table.contains(username)){
        std::cerr << "Error: User not found: " << username << std::endl;
        return;
    }
    
    if (password_to_group == groups_table[groupname].password_hash){
        users_table[username].groups.insert(groupname);
        groups_table[groupname].members.insert(username);
    }
    else{
        std::cerr << "Wrong password to group :" << groupname << ", user : " << username << std::endl;
    }
}

const VirtualFileSystem& VirtualFileSystem::Init(const User& user) {
    VirtualFileSystem* vfs = new VirtualFileSystem();
    MountVFS();

    vfs->active_user = vfs->AddUser(user);

    Group* default_group = new Group("default", "0");
    vfs->AddGroup(*default_group);

    vfs->current_directory = "files";
    return *vfs;
}

bool VirtualFileSystem::IsGroupInSystem(const std::string& groupname) {
    return groups_table.contains(groupname);
}

void VirtualFileSystem::RemoveGroup(const std::string& groupname){
    if(!groups_table.contains(groupname)){
        std::cerr << "Error: Group not found: " << groupname << std::endl;
        return;
    }

    for (const auto& username : groups_table[groupname].members){
        users_table[username].groups.erase(groupname);
    }
    groups_table.erase(groupname);
}

void VirtualFileSystem::CreateFile(const FileInfo& file_info){
    index_table.insert({file_info.GetVirtualPath(), file_info});
    index_table[current_directory].size += 1;
}

void VirtualFileSystem::MoveFile(const std::string& path_from, const std::string& path_to){
    if(!index_table.contains(path_from)){
        std::cerr << "Error: File not found: " << path_from << std::endl;
        return;
    }
    if(!index_table.contains(path_to)){
        std::cerr << "Error: File not found: " << path_to << std::endl;
        return;
    }

    index_table.insert({path_to, index_table[path_from]});
    index_table.erase(path_from);
}

void VirtualFileSystem::RemoveFile(const std::string& filename, bool is_recursive){
    if (!index_table.contains(filename)) {
        std::cerr << "Error: File not found: " << filename << std::endl;
        return;
    }
    if (is_recursive) {
        std::cerr << "Error: Recursive remove not implemented yet" << std::endl;
        return;
    }

    index_table.erase(filename);
}

const FileInfo& VirtualFileSystem::GetFileInfo(const std::string& filename) {
    return index_table[filename];
}

void VirtualFileSystem::GoTo(const std::string& path){
    if (!index_table.contains(path)) {
        std::cerr << "Error: File not found: " << path << std::endl;
        return;
    }
    current_directory = path;
}

void VirtualFileSystem::Shutdown(){
    std::filesystem::path vfs_path(MOUNT_DIR + "/" + VFS_DIR);

    try {
        std::filesystem::remove_all(vfs_path);
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "VFS shutdown: Error removing files: " << e.what() << std::endl;
    }
}

VirtualFileSystem::~VirtualFileSystem(){
    Group* default_group = &(groups_table.at("default"));
    groups_table.erase("default");
    delete default_group;
    Shutdown();
}

};