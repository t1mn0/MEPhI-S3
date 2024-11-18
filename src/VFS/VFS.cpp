
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

#include "../../include/Pair/Pair.hpp"
#include "../../include/VFS/Utils.hpp"
#include "../../include/VFS/VFS.hpp"

namespace tmn_hash {
template<>
unsigned int Hash<tmn_vfs::VirtualFileSystem::Path>(const tmn_vfs::VirtualFileSystem::Path& p) {
    return std::hash<std::string>{}(p.toString());
}
}

namespace tmn_vfs{

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Path :

VirtualFileSystem::Path::Path(const std::string& path): path(path) {}

std::string VirtualFileSystem::Path::ParentFileName() const noexcept {
    size_t first_last_slash = path.find_last_of('/');
    if (first_last_slash == std::string::npos) {
        return "";
    }
    size_t second_last_slash = path.rfind('/', first_last_slash -1);
    if(second_last_slash == std::string::npos){
        return path.substr(0, first_last_slash);
    }
    return path.substr(second_last_slash + 1, first_last_slash - second_last_slash -1);
}

std::string VirtualFileSystem::Path::FileName() const noexcept {
    size_t last_slash = path.find_last_of('/');
    if (last_slash == std::string::npos) {
        return path;
    }
    return path.substr(last_slash + 1);
}

std::string VirtualFileSystem::Path::FileExtension() const noexcept {
    size_t last_dot = path.find_last_of('.');
    if (last_dot == std::string::npos || last_dot < path.find_last_of('/')) {
        return "";
    }
    return path.substr(last_dot + 1);
}

std::string VirtualFileSystem::Path::toString() const{
    return path;
}

bool VirtualFileSystem::Path::operator==(const Path& other) const noexcept {
    return path == other.path;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Support functions for initialization :

tmn_sequence::ArraySequence<std::string> VirtualFileSystem::FindRecordingFiles() noexcept {
    tmn_sequence::ArraySequence<std::string> result;
    std::filesystem::path entry_point(ENTRY_POINT);

    if (!std::filesystem::exists(entry_point) || !std::filesystem::is_directory(entry_point)) {
        std::cerr << "Error(FindRecordingFiles): ENTRY_POINT is not a valid directory." << std::endl;
        return result;
    }


    for (const auto& entry : std::filesystem::recursive_directory_iterator(entry_point)) {
        if (std::filesystem::is_regular_file(entry)) {
            std::string extension = entry.path().extension().string();
            if (AllowedTextExtensions.contains(extension)) {
                result.push_back(entry.path().string());
            }
        }
    }

    return result;
}

bool VirtualFileSystem::VFSInSystem(){
    std::filesystem::path entry_point(ENTRY_POINT);
    return std::filesystem::exists(entry_point / GROUPS) &&
            std::filesystem::exists(entry_point / USERS) &&
            std::filesystem::exists(entry_point / FILE_DESCRIPTORS) &&
            std::filesystem::exists(entry_point / VFS_INFO);
}

void VirtualFileSystem::InstallHelperFiles() {
    std::filesystem::path entry_point(ENTRY_POINT);

    if (std::filesystem::exists(entry_point / GROUPS)) std::filesystem::remove(entry_point / GROUPS);
    if (std::filesystem::exists(entry_point / USERS)) std::filesystem::remove(entry_point / USERS);
    if (std::filesystem::exists(entry_point / FILE_DESCRIPTORS)) std::filesystem::remove(entry_point / FILE_DESCRIPTORS);
    if (std::filesystem::exists(entry_point / VFS_INFO)) std::filesystem::remove(entry_point / VFS_INFO);

    std::ofstream(entry_point / GROUPS);
    std::ofstream(entry_point / USERS);
    std::ofstream(entry_point / FILE_DESCRIPTORS);
    std::ofstream(entry_point / VFS_INFO);
}

void VirtualFileSystem::InitFileDescriptors() {
    std::filesystem::path file_path(ENTRY_POINT);
    file_path = file_path / FILE_DESCRIPTORS;
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error(InitFileDescriptors) : error opening file: " << file_path << std::endl;
        return;
    }

    try {
        nlohmann::json j;
        file >> j;

        for (const auto& fd_json : j) {
            FileDescriptor fd;
            fd.is_dir = fd_json["is_dir"].get<bool>();
            fd.physical_path = fd_json["physical_path"].get<std::string>();
            fd.virtual_path = fd_json["virtual_path"].get<std::string>();
            fd.size = fd_json["size"].get<std::size_t>();
            fd.owner_user = fd_json["owner_user"].get<std::string>();
            fd.owner_group = fd_json["owner_group"].get<std::string>();
            fd.creation_time = fd_json["creation_time"].get<std::string>();
            fd.modification_time = fd_json["modification_time"].get<std::string>();
            fd.descriptor_modification_time = fd_json["descriptor_modification_time"].get<std::string>();

            unsigned int user_perm = fd_json["permissions"]["user"].get<unsigned int>();
            unsigned int group_perm = fd_json["permissions"]["group"].get<unsigned int>();
            unsigned int other_perm = fd_json["permissions"]["other"].get<unsigned int>();
            fd.file_permissions = FilePermissions(static_cast<Permission>(user_perm),static_cast<Permission>(group_perm),static_cast<Permission>(other_perm));

            for(const auto& inner_file: fd_json["inner_files"]){
                fd.inner_files.insert(inner_file.get<std::string>());
            }

            Path path(fd.virtual_path);
            index_table.insert(tmn::Pair<const Path, FileDescriptor>(path, fd));
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
        throw;
    }
}

void VirtualFileSystem::InitGroups() {
    std::filesystem::path file_path(ENTRY_POINT);
    file_path = file_path / GROUPS;
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error(InitGroups): error opening file: " << file_path << std::endl;
        return;
    }

    try {
        nlohmann::json j;
        file >> j;
        for (const auto& group_json : j) {
            Group group(group_json["groupname"], group_json["owner_user"]);
            group.creation_time = group_json["creation_time"];
            for (const auto& member : group_json["members"]) {
                group.members.insert(member);
            }
            groups_table.insert(tmn::Pair<const std::string, Group>(group.groupname, group));
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
        throw;
    }
}

void VirtualFileSystem::InitUsers() {
    std::filesystem::path file_path(ENTRY_POINT);
    file_path = file_path / USERS;
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error(InitUsers): opening file: " << file_path << std::endl;
        return;
    }

    try {
        nlohmann::json j;
        file >> j;
        for (const auto& user_json : j) {
            User user(user_json["username"], user_json["fullname"], user_json["password_hash"], static_cast<UserStatus>(user_json["status"]));
            user.creation_time = user_json["creation_time"];
            for (const auto& group : user_json["groups"]) {
                user.groups.insert(group);
            }
            users_table.insert(tmn::Pair<const std::string, User>(user.username, user));
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
        throw;
    }
}

void VirtualFileSystem::InitSystemParameters(){
    std::filesystem::path file_path(VFS_INFO);
    std::ifstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error(InitSystemParameters): error opening file: " << file_path << std::endl;
        return;
    }

    try {
        nlohmann::json j;
        file >> j;

        active_user = j["active_user"].get<std::string>();

        for (const auto& file_json : j["writable_files"]) {
            writable_files.insert(file_json.get<std::string>());
        }
    } catch (const nlohmann::json::exception& e) {
        std::cerr << "JSON error: " << e.what() << std::endl;
        throw;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Save files :

void VirtualFileSystem::SaveSystemParameters() const noexcept {
    std::filesystem::path file_path(VFS_INFO);
    std::ofstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error(SaveSystemParameters): error with opening file: " << file_path << std::endl; 
        return;
    }

    file.clear();

    nlohmann::json j;
    
    j["active_user"] = active_user;

    nlohmann::json writable_files_json;
    for (const auto& file : writable_files) {
        writable_files_json.push_back(file);
    }
    j["writable_files"] = writable_files_json;

    file << std::setw(4) << j << std::endl;
}

void VirtualFileSystem::SaveDescriptors() const noexcept {
    std::filesystem::path file_path(FILE_DESCRIPTORS);
    std::ofstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error(SaveDescriptors): error with opening file: " << file_path << std::endl; 
        return;
    }

    file.clear();

    nlohmann::json j;
    for (const auto& pair : index_table) {
        j.push_back(pair.second.to_json());
    }

    file << std::setw(4) << j << std::endl;
}

void VirtualFileSystem::SaveGroups() const noexcept {
    std::filesystem::path file_path(GROUPS);
    std::ofstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error(SaveGroups): error with opening file: " << file_path << std::endl; 
        return;
    }

    file.clear();

    nlohmann::json j;
    for (const auto& pair : groups_table) {
        j.push_back(pair.second.to_json());
    }

    file << std::setw(4) << j << std::endl;
}

void VirtualFileSystem::SaveUsers() const noexcept {
    std::filesystem::path file_path(USERS);
    std::ofstream file(file_path);

    if (!file.is_open()) {
        std::cerr << "Error(SaveUsers): error with opening file: " << file_path << std::endl; 
        return;
    }

    file.clear();

    nlohmann::json j;
    for (const auto& pair : users_table) {
        j.push_back(pair.second.to_json());
    }

    file << std::setw(4) << j << std::endl;
}

void VirtualFileSystem::SaveAll() const noexcept {
    SaveSystemParameters();
    SaveDescriptors();
    SaveUsers();
    SaveGroups();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Checkers :

const std::string& VirtualFileSystem::WhoUser() const {
    return active_user;
}

const std::string& VirtualFileSystem::PWD() const {
    return current_directory;
}

std::size_t VirtualFileSystem::CountUser() const {
    return users_table.size();
}

std::size_t VirtualFileSystem::CountGroup() const {
    return groups_table.size();
}

std::size_t VirtualFileSystem::CountMembers(const std::string& groupname) const {
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

bool VirtualFileSystem::IsPathInSystem(const Path& path) const {
    return index_table.contains(path);
}

bool VirtualFileSystem::IsMember(const std::string& groupname, const std::string& username) const{
    if (groups_table.contains(groupname)){
        return groups_table[groupname].members.contains(username);
    }
    else{
        std::cerr << "Error(IsMember): Group not found: " << groupname << std::endl;
        return false;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Users policy & actions :

void VirtualFileSystem::AddUser(const User& user) {
    if(users_table.contains(user.username)){
        std::cerr << "Error(AddUser): User with name '"<< user.username <<"' already is in system" << std::endl;
    }
    else{
        users_table.insert(tmn::Pair<const std::string, User>(user.username, user));
        users_table[user.username].groups.insert(user.username);
        groups_table.insert(tmn::Pair<const std::string, Group>(user.username, Group()));
    }
}

void VirtualFileSystem::Authorization(const std::string& username, const std::string& password_hash){
    if(!users_table.contains(username)){
        std::cerr << "Error(Authorization): User not found: " << username << std::endl;
    }
    if(users_table[username].password_hash == password_hash){
        active_user = username;
        active_super = 5;
    }
}

void VirtualFileSystem::RemoveUser(const std::string& username){
    if(!users_table.contains(username)){
        std::cerr << "Error(RemoveUser): User not found: " << username << std::endl;
    }
    else if (username == active_user){
        std::cerr << "Error(RemoveUser): User is active: " << username << std::endl;
    }

    for (const auto& groupname : users_table[username].groups){
        groups_table[groupname].members.erase(username);
    }   
    users_table.erase(username);
    groups_table.erase(username);
}

tmn::Optional<User> VirtualFileSystem::GetUserInfo(const std::string& username) const {
    if (users_table.contains(username)){
        return tmn::Optional<User>(users_table[username]);
    }
    return tmn::Optional<User>();
}

tmn_sequence::ArraySequence<std::string> VirtualFileSystem::AllUsers() const {
    return users_table.keys();
}

tmn_associative::HashSet<std::string> VirtualFileSystem::AllUserGroups(const std::string& username) const{
    tmn_associative::HashSet<std::string> set;
    if (users_table.contains(username)){
        set = users_table[username].groups;
    }
    return set;
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

void VirtualFileSystem::AddToGroup(const std::string& username, const std::string& groupname){
    if(!groups_table.contains(groupname)){
        std::cerr << "Error(AddToGroup): Group not found: " << groupname << std::endl;
        return;
    }
    if(!users_table.contains(username)){
        std::cerr << "Error(AddToGroup): User not found: " << username << std::endl;
        return;
    }
    
    users_table[username].groups.insert(groupname);
    groups_table[groupname].members.insert(username);
}

void VirtualFileSystem::RemoveFromGroup(const std::string& username, const std::string& groupname) {
    if (groupname == username){
        std::cerr << "Error(RemoveFromGroup): Never delete user from default user-group!" << std::endl;
        return;
    }
    if (groups_table.contains(groupname)){
        if (groups_table[groupname].members.contains(username)){
            groups_table[groupname].members.erase(username);
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

tmn::Optional<Group> VirtualFileSystem::GetGroupInfo(const std::string& groupname) const {
    if (users_table.contains(groupname)){
        return tmn::Optional<Group>(groups_table[groupname]);
    }
    return tmn::Optional<Group>();
}

tmn_sequence::ArraySequence<std::string> VirtualFileSystem::GetAllGroups() const {
    return groups_table.keys();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Actions with files :

// tmn_associative::HashSet<std::string> VirtualFileSystem::CurrentDirContent() {
//     return index_table[Path(current_directory)].inner_files;
// }

// void VirtualFileSystem::AddFile(const FileDescriptor& file_info){
//     index_table.insert(tmn::Pair<const std::string, FileDescriptor>(file_info.virtual_path, file_info));
//     index_table[GetParentPath(file_info.virtual_path)].inner_files.insert(file_info.virtual_path);
// }

// // void VirtualFileSystem::MoveFile(const std::string& path_from, const std::string& path_to){
// //     if(!index_table.contains(path_from)){
// //         std::cerr << "Error(MoveFile): File not found: " << path_from << std::endl;
// //         return;
// //     }
// //     if(!index_table.contains(path_to)){
// //         std::cerr << "Error(MoveFile): File not found: " << path_to << std::endl;
// //         return;
// //     }

// //     index_table.insert(tmn::Pair<const std::string, FileDescriptor>(path_to, index_table[path_from]));
// //     index_table.erase(path_from);
// // }

// // void VirtualFileSystem::RemoveFile(const std::string& filename, bool is_recursive){
// //     if (!index_table.contains(filename)) {
// //         std::cerr << "Error(RemoveFile): File not found: " << filename << std::endl;
// //         return;
// //     }
// //     if (is_recursive) {
// //         std::cerr << "Error(RemoveFile): Recursive remove not implemented yet" << std::endl;
// //         return;
// //     }

// //     index_table.erase(filename);
// // }

// const FileDescriptor& VirtualFileSystem::GetFileInfo(const std::string& filename) {
//     return index_table[filename];
// }

// // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// // Modifiers :

// void VirtualFileSystem::GoTo(const std::string& path){
//     if (!index_table.contains(path)) {
//         std::cerr << "Error(GoTo): File not found: " << path << std::endl;
//         return;
//     }
//     current_directory = path;
// }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Initializing and shutdown:

VirtualFileSystem VirtualFileSystem::Init() {
    VirtualFileSystem vfs;
    if (VFSInSystem()){
        vfs.InitFileDescriptors();
        vfs.InitGroups();
        vfs.InitUsers();
        vfs.InitSystemParameters();
        // ! add autorization -> active_user
    }
    else{
        InstallHelperFiles();
    }

    return vfs;
}

VirtualFileSystem::~VirtualFileSystem(){
    SaveAll();
    index_table.clear();
    groups_table.clear();
    users_table.clear();
}


};