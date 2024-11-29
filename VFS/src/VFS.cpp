
#include <fstream>
#include <filesystem>
#include <string>
#include <iostream>

#include "../include/VFS.hpp"
#include "../include/Utils.hpp"
#include "../../include/Adapter/Stack.hpp"

namespace tmn_vfs{

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Checkers :

std::string VirtualFileSystem::PWD() const {
    if (current_directory == 0) return "/";
    tmn_adapter::Stack<std::string> stack;
    unsigned long fid = current_directory; 
    stack.push(files[fid].filename);
    while (fid != 0){
        fid = files[fid].parent_dir_id;
        stack.push(files[fid].filename);
    }
    stack.pop();
    std::string path = "/";
    while (!stack.empty()){
        path += stack.top();
        path += "/";
        stack.pop();
    }
    path.pop_back();
    return path;
}

std::size_t VirtualFileSystem::CountUser() const {
    return users_table.size();
}

std::size_t VirtualFileSystem::CountGroup() const {
    return groups_table.size();
}

std::size_t VirtualFileSystem::CountMembers(const std::string& groupname) const {
    if (groupnames.contains(groupname)) {
        return groups_table[groupnames[groupname]].members.size();
    }
    std::cerr << "Error(CountMembers): Group not found: " << groupname << std::endl;
    return 0;
}

bool VirtualFileSystem::IsUserInSystem(const std::string& username) const {
    if (usernames.contains(username)) {
        return users_table.contains(usernames[username]);
    }
    std::cerr << "Error(IsUserInSystem): User not found: " << username << std::endl;
    return false;
}

bool VirtualFileSystem::IsGroupInSystem(const std::string& groupname) const {
    if (groupnames.contains(groupname)) {
        return groups_table.contains(groupnames[groupname]);
    }
    std::cerr << "Error(IsGroupInSystem): Group not found: " << groupname << std::endl;
    return false;
}

bool VirtualFileSystem::IsFileInCurrentDir(const std::string& filename) const {
    for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == filename){
            return true;
        }
    }
    return false;
}

bool VirtualFileSystem::IsMember(const std::string& groupname, const std::string& username) const {
    if (usernames.contains(username)) {
        if (groupnames.contains(groupname)) {
            return groups_table[groupnames[groupname]].members.contains(usernames[username]);;
        }
        std::cerr << "Error(IsMember): Group not found: " << groupname << std::endl;
        return false;
    }
    std::cerr << "Error(IsMember): User not found: " << username << std::endl;
    return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Other methods :

unsigned long VirtualFileSystem::NextRecordFile() noexcept {
    ++recording_file;
    if (recording_file >= recording_files.size()){
        recording_file = 0;
    }
    return recording_file;
}

tmn::Optional<std::string> VirtualFileSystem::DoPath(unsigned long id) const noexcept {
    if (!files.contains(id)){
        std::cerr << "Error(DoPath): Argument is invalid" << std::endl;
        return tmn::Optional<std::string>();
    }
    std::string path = files[id].filename;
    path += "/";
    unsigned long current_file_id = id;

    while (current_file_id != 0){
        if (!files.contains(current_file_id)){
            std::cerr << "Error(DoPath): Argument is invalid" << std::endl;
            return tmn::Optional<std::string>();
        }
        path += files[current_file_id].filename;
        path += "/";
        current_file_id = files[current_file_id].parent_dir_id;
    }

    path = ReversePath(path);
    return tmn::Optional<std::string>(path);
}

bool VirtualFileSystem::GoTo(std::string& path) {
    if (path.empty()){
        std::cerr << "Error(GoTo): Path is invalid" << std::endl;
        return false;
    }
    if (path == "/"){
        current_directory = 0;
        return true;
    }

    unsigned long init_dir = current_directory;
    unsigned long iter_dir;

    tmn_sequence::ArraySequence<std::string> components;
    size_t pos = 0;
    std::string token;
    while ((pos = path.find('/')) != std::string::npos) {
        token = path.substr(0, pos);
        components.push_back(token);
        path.erase(0, pos + 1);
    }

    if (!path.empty()) components.push_back(path);
    if (components[0] == ".." && iter_dir != 0){
        iter_dir = files[current_directory].parent_dir_id;
    }
    else{
        iter_dir = current_directory;
    }

    for (std::size_t i = 0; i < components.size(); ++i) {
        if (components[i] == ".."){
            if (files.contains(files[iter_dir].parent_dir_id) && files[files[iter_dir].parent_dir_id].is_dir){
                iter_dir = files[iter_dir].parent_dir_id;
            }
            else{
                std::cerr << "Error(GoTo): Path is invalid" << std::endl;
                current_directory = init_dir;
                return false;
            }
        }
        else{
            bool flag = false; 
            for (auto& inner_fid : files[iter_dir].inner_files){
                if (files[inner_fid].filename == components[i]){
                    if (files[inner_fid].is_dir){
                        iter_dir = inner_fid;
                        flag = true;
                        break;
                    }
                    else{
                        std::cerr << "Error(GoTo): Path is invalid" << std::endl;
                        current_directory = init_dir;
                        return false;
                    }
                }
            }
            if (!flag) {
                std::cerr << "Error(GoTo): Path is invalid" << std::endl;
                current_directory = init_dir;
                return false;
            }
        }
    }

    current_directory = iter_dir;
    return true;
}

};