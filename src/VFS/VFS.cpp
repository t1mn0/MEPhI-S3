
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>

#include "../../include/Pair/Pair.hpp"
#include "../../include/VFS/Utils.hpp"
#include "../../include/VFS/VFS.hpp"


namespace tmn_vfs{

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Checkers :

const std::string& VirtualFileSystem::WhoUser() const {
    return active_user;
}

const std::string &tmn_vfs::VirtualFileSystem::PWD() const {
    return current_directory;
}

std::size_t tmn_vfs::VirtualFileSystem::CountUser() const {
    return users_table.size();
}

std::size_t tmn_vfs::VirtualFileSystem::CountGroup() const {
    return groups_table.size();
}

std::size_t tmn_vfs::VirtualFileSystem::CountMembers(const std::string& groupname) const {
    if(groups_table.contains(groupname)){
        return groups_table[groupname].members.size();
    }
    else{
        std::cerr << "Error(CountMembers): Group '"<< groupname <<"' is not found" << std::endl;
        return 0;
    }
}

bool VirtualFileSystem::IsUserInSystem(const std::string& username) const {
    return users_table.contains(username);
}

bool VirtualFileSystem::IsGroupInSystem(const std::string& groupname) const {
    return groups_table.contains(groupname);
}

bool tmn_vfs::VirtualFileSystem::IsMember(const std::string& groupname, const std::string& username) const{
    if (groups_table.contains(groupname)){
        return groups_table[groupname].members.contains(username);
    }
    else{
        std::cerr << "Error(IsMember): Group not found: " << groupname << std::endl;
        return false;
    }
}

bool tmn_vfs::VirtualFileSystem::IsGoodPath(const std::string& path) const {
    return index_table.contains(path);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Users policy & actions :

void tmn_vfs::VirtualFileSystem::AddUser(const User& user) {
    if(users_table.contains(user.GetUserName())){
        std::cerr << "Error(AddUser): User '"<< user.username <<"' already is in system" << std::endl;
    }
    else{
        users_table.insert(tmn::Pair<const std::string, User>(user.username, user));
        users_table[user.username].groups.insert("default");
        groups_table["default"].members.insert(user.username);
    }
}

void VirtualFileSystem::Authorization(const std::string& username, const std::string& password_hash){
    if(!users_table.contains(username)){
        std::cerr << "Error(Authorization): User not found: " << username << std::endl;
        return;
    }
    if(users_table[username].password_hash == password_hash){
        active_user = username;
    }
}

void VirtualFileSystem::RemoveUser(const std::string& username){
    if(!users_table.contains(username)){
        std::cerr << "Error(RemoveUser): User not found: " << username << std::endl;
        return;
    }
    else if (username == active_user){
        std::cerr << "Error(RemoveUser): User is active: " << username << std::endl;
        return;
    }

    for (const auto& groupname : users_table[username].groups){
        groups_table[groupname].members.erase(username);
    }   
    users_table.erase(username);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Groups policy & actions :

void VirtualFileSystem::AddGroup(const Group& group){
    if(groups_table.contains(group.groupname)){
        std::cerr << "Error(AddGroup): Group '"<< group.groupname <<"' is already in system" << std::endl;
    }
    else{
        groups_table.insert(tmn::Pair<const std::string, Group>(group.groupname, group));
    }
}

void VirtualFileSystem::AddToGroup(const std::string& username, const std::string& groupname, const std::string& password_hash){
    if(!groups_table.contains(groupname)){
        std::cerr << "Error(AddToGroup): Group not found: " << groupname << std::endl;
        return;
    }
    if(!users_table.contains(username)){
        std::cerr << "Error(AddToGroup): User not found: " << username << std::endl;
        return;
    }
    
    if (password_hash == groups_table[groupname].password_hash){
        users_table[username].groups.insert(groupname);
        groups_table[groupname].members.insert(username);
    }
    else{
        std::cerr << "(AddToGroup) Wrong password to group: " << groupname << ", user: " << username << std::endl;
    }
}

void tmn_vfs::VirtualFileSystem::RemoveFromGroup(const std::string& username, const std::string& groupname, const std::string& password_hash) {
    if (groupname == "default"){
        std::cerr << "Error(RemoveFromGroup): Never delete user from default group!" << std::endl;
        return;
    }
    if (groups_table.contains(groupname)){
        if (groups_table[groupname].members.contains(username)){
            if (groups_table[groupname].password_hash == password_hash){
                groups_table[groupname].members.erase(username);
            }
            else{
                std::cerr << "Error(RemoveFromGroup): Wrong password to group: " << groupname << std::endl;
                return;
            }
        }
        else{
            std::cerr << "Error(RemoveFromGroup): User not found: " << username << std::endl;
            return;
        }
    }
    else{
        std::cerr << "Error(RemoveFromGroup): Group not found: " << groupname << std::endl;
        return;
    }
}

void VirtualFileSystem::RemoveGroup(const std::string& groupname){
    if(groupname == "default"){
        std::cerr << "(RemoveGroup) Never delete the default group" << std::endl;
        return;
    }

    if(!groups_table.contains(groupname)){
        std::cerr << "Error(RemoveGroup): Group not found: " << groupname << std::endl;
        return;
    }

    for (const auto& username : groups_table[groupname].members){
        users_table[username].groups.erase(groupname);
    }
    groups_table.erase(groupname);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Actions with files :

void VirtualFileSystem::AddFile(const FileInfo& file_info){
    index_table.insert(tmn::Pair<const std::string, FileInfo>(file_info.virtual_path, file_info));
    index_table[GetParentPath(file_info.virtual_path)].inner_files.insert(file_info.virtual_path);
}

// void VirtualFileSystem::MoveFile(const std::string& path_from, const std::string& path_to){
//     if(!index_table.contains(path_from)){
//         std::cerr << "Error(MoveFile): File not found: " << path_from << std::endl;
//         return;
//     }
//     if(!index_table.contains(path_to)){
//         std::cerr << "Error(MoveFile): File not found: " << path_to << std::endl;
//         return;
//     }

//     index_table.insert(tmn::Pair<const std::string, FileInfo>(path_to, index_table[path_from]));
//     index_table.erase(path_from);
// }

// void VirtualFileSystem::RemoveFile(const std::string& filename, bool is_recursive){
//     if (!index_table.contains(filename)) {
//         std::cerr << "Error(RemoveFile): File not found: " << filename << std::endl;
//         return;
//     }
//     if (is_recursive) {
//         std::cerr << "Error(RemoveFile): Recursive remove not implemented yet" << std::endl;
//         return;
//     }

//     index_table.erase(filename);
// }

const FileInfo& VirtualFileSystem::GetFileInfo(const std::string& filename) {
    return index_table[filename];
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Modifiers :

void VirtualFileSystem::GoTo(const std::string& path){
    if (!index_table.contains(path)) {
        std::cerr << "Error(GoTo): File not found: " << path << std::endl;
        return;
    }
    current_directory = path;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Initializing and shutdown:

void VirtualFileSystem::MountVFS() {
    std::filesystem::path vfs_path(MOUNT_DIR + "/" + VFS_DIR);

    std::filesystem::create_directories(vfs_path / "files");

    for (int i = 0; i < FILE_COUNT; ++i) {
        std::ofstream file(vfs_path / "files" / ("File-" + std::to_string(i) + ".txt"));
        file.close();
    }
}

VirtualFileSystem VirtualFileSystem::Init(const User& user) {
    VirtualFileSystem vfs;
    MountVFS();
    vfs.users_table.insert(tmn::Pair<const std::string, User>(user.username, user));

    vfs.active_user = user.username;

    Group default_group("default", "0");
    default_group.members.insert(user.username);
    vfs.AddGroup(default_group);

    return vfs;
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
    Shutdown();
    index_table.clear();
    groups_table.clear();
    users_table.clear();
}


};