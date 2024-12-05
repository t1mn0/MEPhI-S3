
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>

#include "../include/VFS.hpp"
#include "../include/Utils.hpp"
#include "../../include/Exceptions/RuntimeException.hpp"
#include "../../include/Adapter/Stack.hpp"

namespace tmn_vfs{

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Checkers :

std::string VirtualFileSystem::PWD() const noexcept {
    if (current_directory == 0) return "/";

    tmn_adapter::Stack<std::string> stack;
    unsigned long fid = current_directory; 
    stack.push(files[fid].filename);

    while (fid != 0){
        fid = files[fid].parent_dir_id;
        stack.push(files[fid].filename);
    }

    stack.pop(); // for root-dir '/'
    std::string path = "/";

    while (!stack.empty()){
        path += stack.top();
        path += "/";
        stack.pop();
    }
    path.pop_back(); // for last '/'
    return path;
}

std::size_t VirtualFileSystem::CountUser() const noexcept {
    return users_table.size();
}

std::size_t VirtualFileSystem::CountGroup() const noexcept {
    return groups_table.size();
}

tmn::Optional<std::size_t> VirtualFileSystem::CountMembers(const std::string& groupname) const noexcept {
    if (groupnames.contains(groupname)) {
        return tmn::Optional<std::size_t>(groups_table[groupnames[groupname]].members.size());
    }
    
    return tmn::Optional<std::size_t>();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Other methods :

void VirtualFileSystem::GoTo(std::string& path) {
    if (path.empty()){
        std::string err_message = "Error(GoTo): Path is invalid";
        throw tmn_exception::RuntimeException(err_message);
    }

    if (path == "/"){
        current_directory = 0;
        return;
    }

    // If an error occurs during execution, you must return to the original directory
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

    if (components[0] == ".." && current_directory != 0){
        iter_dir = files[current_directory].parent_dir_id;
    }
    else{
        iter_dir = current_directory;
    }

    for (std::size_t i = 0; i < components.size(); ++i) {
        if (components[i] == ".."){
            if (iter_dir != 0){
                if (files.contains(files[iter_dir].parent_dir_id) && files[files[iter_dir].parent_dir_id].is_dir){
                    iter_dir = files[iter_dir].parent_dir_id;
                }
                else{
                    current_directory = init_dir;
                    std::string err_message = "Error(GoTo): Path is invalid";
                    throw tmn_exception::RuntimeException(err_message);
                }
            }
            else{
                current_directory = init_dir;
                std::string err_message = "Error(GoTo): Path is invalid";
                throw tmn_exception::RuntimeException(err_message);
            }
        }
        else{
            bool flag = false; 
            for (auto& inner_fid : files[iter_dir].inner_files){
                if (files[inner_fid].filename == components[i]){
                    if (!HavePermission(inner_fid, active_user, 1)){
                        current_directory = init_dir;
                        std::string err_message = "Error(GetContent): permission denied";
                        throw tmn_exception::RuntimeException(err_message);
                    }
                    
                    if (files[inner_fid].is_dir){
                        iter_dir = inner_fid;
                        flag = true;
                        break;
                    }
                    else{
                        current_directory = init_dir;
                        std::string err_message = "Error(GoTo): Path is invalid";
                        throw tmn_exception::RuntimeException(err_message);
                    }
                }
            }
            if (!flag) {
                std::string err_message = "Error(GoTo): Path is invalid";
                throw tmn_exception::RuntimeException(err_message);
            }
        }
    }

    current_directory = iter_dir;
}

tmn::Optional<std::string> VirtualFileSystem::DoPath(unsigned long id) const noexcept {
    if (!files.contains(id)){
        return tmn::Optional<std::string>();
    }
    
    std::string path = files[id].filename + "/";

    unsigned long current_file_id = id;

    while (current_file_id != 0){
        if (!files.contains(current_file_id)){
            return tmn::Optional<std::string>();
        }

        path += files[current_file_id].filename;
        path += "/";
        current_file_id = files[current_file_id].parent_dir_id;
    }

    // “reverse” process for the path:
    std::stringstream ss(path);
    std::string item;
    
    tmn_sequence::ArraySequence<std::string> components;

    while (std::getline(ss, item, '/')) {
        components.push_back(item);
    }

    std::stringstream reversed_path;

    if (!components.empty()){
        for (size_t i = components.size() - 1; i > 0; --i) {
            reversed_path << "/" << components[i];
        }
    }

    return tmn::Optional<std::string>(reversed_path.str());
}

tmn_sequence::ArraySequence<unsigned long> VirtualFileSystem::FindFileByName(const std::string& filename, bool in_current_dir) const noexcept {
    tmn_sequence::ArraySequence<unsigned long> result;
    
    if (in_current_dir){
        for (auto& inner_fd_id : files[current_directory].inner_files){
            if (FileNameMatch(files[inner_fd_id].filename, filename)){
                result.push_back(inner_fd_id);
            }
        }
    }
    else{
        for (auto& fd : files){
            if (FileNameMatch(fd.second.filename, filename)){
                result.push_back(fd.first);
            }
        }
    }

    return result;
}

// perm = 1 -> read, perm = 2 -> write, etc
bool VirtualFileSystem::HavePermission(unsigned long fd_id, unsigned long user_id, unsigned int perm) const {
    if (!files.contains(fd_id)){
        std::string err_message = "Error(CanRead): file not found in current directory";
        throw tmn_exception::RuntimeException(err_message);
    }

    if (!users_table.contains(user_id)){
        std::string err_message = "Error(CanRead): user not found";
        throw tmn_exception::RuntimeException(err_message);
    }

    if (user_id == 0){ // SUPER
        return true;
    }
    
    if(files[fd_id].owner_user == user_id){
        return static_cast<unsigned int>(files[fd_id].file_permissions.user) >= perm;
    }
    
    if (groups_table[files[fd_id].owner_group].members.contains(user_id)){
        return static_cast<unsigned int>(files[fd_id].file_permissions.group) >= perm;
    }

    return static_cast<unsigned int>(files[fd_id].file_permissions.other) >= perm;
}

};