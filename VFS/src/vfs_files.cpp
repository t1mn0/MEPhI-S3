#include <fstream>
#include <filesystem>
#include <string>

#include "../include/VFS.hpp"
#include "../include/Utils.hpp"
#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn_vfs {

tmn_associative::HashSet<std::string> VirtualFileSystem::current_dir_content() const noexcept {
    tmn_associative::HashSet<std::string> set;

    for (auto& fid : files[current_directory].inner_files){
        set.insert(files[fid].filename);
    }

    return set;
}

void VirtualFileSystem::add_file_content(const std::string& filename, const std::string& content) {
    uint64_t fd_id = 0;
    for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == filename){
            fd_id = inner_file;
            if (files[inner_file].content_size != 0){
                std::string err_message = "Error(add_file_content): For 1 virtual file there is 1 physical file";
                throw tmn_exception::RuntimeException(err_message);
            }
            break;
        }
    }
    
    if (fd_id == 0){
        std::string err_message = "Error(add_file_content): Bad filename: " + filename;
        throw tmn_exception::RuntimeException(err_message);
    }

    if (files[fd_id].is_dir){
        std::string err_message = "Error(add_file_content): The specified file is not a regular file. Impossible to write content";
        throw tmn_exception::RuntimeException(err_message);
    }

    if (!have_permission(fd_id, active_user, 2)){
        std::string err_message = "Error(add_file_content): Permission denied";
        throw tmn_exception::RuntimeException(err_message);
    }

    // std::stringstream ss;
    // ss /* << files[fd_id].fd_id << "_" */ << content;
    // std::string content_with_id = ss.str();

    std::fstream file(recording_files[files[fd_id].physical_file_id], std::ios::out | std::ios::binary | std::ios::app);

    if (!file.is_open()) {
        std::string err_message = "Error(add_file_content): Failed to open file: " + recording_files[files[fd_id].physical_file_id];
        throw tmn_exception::RuntimeException(err_message);
    }

    files[fd_id].content_offset = file.tellp();

    file.write(content.c_str(), content.length());

    files[fd_id].content_size = static_cast<uint64_t>(file.tellg()) - files[fd_id].content_offset;
    file.close();
}

std::string tmn_vfs::VirtualFileSystem::get_file_content(const std::string& filename) {
    for (auto& inner_id : files[current_directory].inner_files){
        if (files[inner_id].filename == filename){
            
            if (!have_permission(inner_id, active_user, 1)){
                std::string err_message = "Error(get_file_content): Permission denied";
                throw tmn_exception::RuntimeException(err_message);
            }

            if (!files[inner_id].is_dir){
                std::ifstream file(recording_files[files[inner_id].physical_file_id], std::ios::binary);

                if (!file.is_open()) {
                    std::string err_message = "Error(get_file_content): Could not open file (problem with std::fstream)";
                    throw tmn_exception::RuntimeException(err_message);
                }

                file.seekg(0, std::ios::end);

                file.seekg(files[inner_id].content_offset);

                std::vector<char> buffer(files[inner_id].content_size);
                file.read(buffer.data(), files[inner_id].content_size);

                std::string result(buffer.data(), buffer.size());
                size_t nullbyte_pos = result.find('\0');
                
                if (nullbyte_pos != std::string::npos) {
                    result = result.substr(0, nullbyte_pos);
                }

                file.close();
                return result;
            }
            std::string err_message = "Error(get_file_content): File '" + filename + "' is not regular file";
            throw tmn_exception::RuntimeException(err_message);
        }
    }
    std::string err_message = "Error(get_file_content): File '" + filename + "' not found in specified directory";
    throw tmn_exception::RuntimeException(err_message);
}

void VirtualFileSystem::change_file_permissions(const std::string& filename, uint8_t perm) {
    uint64_t fd_id = 0;
    bool flag = false;
    for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == filename){
            fd_id = inner_file;
            flag = true;
            break;
        }
    }

    if(!flag){
        std::string err_message = "Error(change_file_permissions): File '" + filename + "' not found in current directory";
        throw tmn_exception::RuntimeException(err_message);
    }

    if (fd_id == 0) {
        std::string err_message = "Error(change_file_permissions): Unable to change root directory permissions";
        throw tmn_exception::RuntimeException(err_message);
    }

    if (active_user != files[fd_id].owner_user && active_user != 0){
        std::string err_message = "Error(change_file_permissions): Active user '" + users_table[active_user].username + "' is not the owner of this file/dir: " + filename;
        throw tmn_exception::RuntimeException(err_message);
    }

    if (perm > 999) {
        std::string err_message = "Error(change_file_permissions): Permissions value must be a three-digit number";
        throw tmn_exception::RuntimeException(err_message);
    }

    if (perm / 100 > 3 || (perm / 10) % 10 > 3 || perm % 10 > 3){
        std::string err_message = "Error(change_file_permissions): Bad digit performance for permission";
        throw tmn_exception::RuntimeException(err_message);
    }
    
    Permission user_perm = static_cast<Permission>(perm / 100);
    Permission group_perm = static_cast<Permission>((perm / 10) % 10);
    Permission other_perm = static_cast<Permission>(perm % 10);

    files[fd_id].file_permissions = FilePermissions(user_perm, group_perm, other_perm);
}

// add_file(FileDescriptor& fd) - sets the content_offset, content_size, owner_user, owner_group, parent_dir_id fields for fd
void VirtualFileSystem::add_file(FileDescriptor fd) {
    fd.owner_user = active_user;
    fd.owner_group = active_user;
    fd.parent_dir_id = current_directory;

    if (!have_permission(current_directory, active_user, 2)){
        std::string err_message = "Error(add_file): permission denied";
        throw tmn_exception::RuntimeException(err_message);
    }

    for (auto& inner_file : files[current_directory].inner_files){
        if (fd.filename == files[inner_file].filename){
            std::string err_message = "Error(add_file): File with this name already exists in this directory";
            throw tmn_exception::RuntimeException(err_message);
        }
    }

    files[current_directory].inner_files.insert(fd.fd_id);
    files.insert({fd.fd_id, fd});
}

void VirtualFileSystem::rename_file(const std::string& old_filename, const std::string& new_filename){
    for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == new_filename){
            std::string err_message = "Error(rename_file): In current directory there is already a file with that name: " + new_filename;
            throw tmn_exception::RuntimeException(err_message);
        }
    }

    for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == old_filename){
            if(files[inner_file].owner_user != active_user){
                std::string err_message = "Error(rename_file): Active user '" + users_table[active_user].username + "' is not the owner of this file/dir: " + old_filename;
                throw tmn_exception::RuntimeException(err_message);
            }
            files[inner_file].filename = new_filename;
        }
    }

    std::string err_message = "Error(rename_file): File '" + old_filename + "' not found in current directory";
    throw tmn_exception::RuntimeException(err_message);
}

void VirtualFileSystem::set_owner_group(uint64_t fd_id, uint64_t group_id) {
    if (!files.contains(fd_id)){
        std::string err_message = "Error(rename_file): File not found in current directory";
        throw tmn_exception::RuntimeException(err_message);
    }

    if (!groups_table.contains(group_id)){
        std::string err_message = "Error(set_owner_group): Group not found";
        throw tmn_exception::RuntimeException(err_message);
    }

    if(files[fd_id].owner_user != active_user){
        std::string err_message = "Error(set_owner_group): Active user '" + users_table[active_user].username + "' is not the owner of this file";
        throw tmn_exception::RuntimeException(err_message);
    }

    files[fd_id].owner_group = group_id;
}

void VirtualFileSystem::remove_file_content(const std::string& filename) {
    uint64_t id = 0;
     for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == filename){
            if (files[inner_file].is_dir){
                std::string err_message = "Error(remove_file_content): File for removing content is not regular " + filename;
                throw tmn_exception::RuntimeException(err_message);
            }
            else{
                id = inner_file;
                break;
            }
        }
    }

    std::fstream file(recording_files[files[id].physical_file_id], std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) {
        std::string err_message = "Error(remove_file_content): Failed to open file: " + recording_files[files[fd_id].physical_file_id];
        throw tmn_exception::RuntimeException(err_message);
    }

    file.seekg(0, std::ios::end);
    uint64_t file_size = static_cast<uint64_t>(file.tellg());

    if (file_size < files[id].content_size) {
        file.close();
        std::string err_message = "Error(remove_file_content): File is smaller than the number of bytes to remove";
        throw tmn_exception::RuntimeException(err_message);
    }
    
    file.seekp(file_size - files[id].content_size);
    std::filesystem::resize_file(std::filesystem::path(recording_files[files[id].physical_file_id]), file_size - files[id].content_size);

    file.close();
}

void VirtualFileSystem::remove_file(const std::string& filename) {
    for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == filename){
            if (!files[inner_file].is_dir){
                try {
                    remove_file_content(filename);
                }
                catch(tmn_exception::RuntimeException& e){
                    throw e;
                }

                if(files[inner_file].owner_user != active_user){
                    std::string err_message = "Error(remove_file): Active user '" + users_table[active_user].username +"' is not the owner of this file: " + filename;
                    throw tmn_exception::RuntimeException(err_message);
                }

                files[current_directory].inner_files.erase(inner_file);
                files.erase(inner_file);
                return;
            }
            else{
                std::string err_message = "Error(remove_file): file '" + filename + "' is directory. Try to use remove_dir";
                throw tmn_exception::RuntimeException(err_message);
            }
        }
    }
    
    std::string err_message = "Error(remove_file): file '" + filename + "' not found in current directory";
    throw tmn_exception::RuntimeException(err_message);
}

void VirtualFileSystem::remove_dir(const std::string& filename, bool is_recursive){
    for (auto& matched_file : files[current_directory].inner_files){
        // Iteration-searching for a directory
        if (files[matched_file].filename == filename){
            if (files[matched_file].is_dir){
                if(files[matched_file].owner_user != active_user){
                    std::string err_message = "Error(remove_dir): Active user '" + users_table[active_user].username +"' is not the owner of this directory: " + filename;
                    throw tmn_exception::RuntimeException(err_message);
                }

                if (!is_recursive) {
                    // Iteration-check before deletion :
                    for (uint64_t inner_fd_id : files[matched_file].inner_files) {
                        if (files[inner_fd_id].is_dir && files[inner_fd_id].inner_files.empty()){
                            std::string err_message = 
                            "Error(remove_file): You cannot delete a directory with non-empty subdirectories: '" + 
                                filename + "/" + files[inner_fd_id].filename  + "'\n" +
                            "You can use flag 'recursive' to remove all internal content";

                            throw tmn_exception::RuntimeException(err_message);
                        }
                    }
                    // Iteration with removing files :
                    for (uint64_t inner_fd_id : files[matched_file].inner_files) {
                        files[matched_file].inner_files.erase(inner_fd_id);
                        files.erase(inner_fd_id);
                    }
                }
                else{
                    for (uint64_t inner_fd_id : files[matched_file].inner_files) {
                        if (files[inner_fd_id].is_dir && files[inner_fd_id].inner_files.empty() || !files[inner_fd_id].is_dir) {
                            files[matched_file].inner_files.erase(inner_fd_id);
                            files.erase(inner_fd_id);
                        }
                        else{
                            try {
                                remove_dir(files[inner_fd_id].filename, true);
                            }
                            catch (tmn_exception::RuntimeException& e){
                                throw e;
                            }
                        }
                    }
                }
                
                files.erase(files[matched_file].parent_dir_id);
                files.erase(matched_file);
            }
            else {
                std::string err_message = "Error(remove_file): File '" + filename + "' is not directory";
                throw tmn_exception::RuntimeException(err_message);
            }
        }
    }

    std::string err_message = "Error(remove_file): File '" + filename + "' not found in current directory";
    throw tmn_exception::RuntimeException(err_message);
}

}