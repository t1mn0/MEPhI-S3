#include <fstream>
#include <filesystem>
#include <string>

#include "../include/VFS.hpp"
#include "../include/Utils.hpp"
#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn_vfs {

tmn_associative::HashSet<std::string> VirtualFileSystem::CurrentDirContent() const noexcept {
    tmn_associative::HashSet<std::string> set;

    for (auto& fid : files[current_directory].inner_files){
        set.insert(files[fid].filename);
    }

    return set;
}

// AddFile(FileDescriptor& fd) - sets the content_offset, content_size, owner_user, owner_group, parent_dir_id fields for fd
void VirtualFileSystem::AddFile(FileDescriptor fd) {
    fd.owner_user = active_user;
    fd.owner_group = active_user;
    fd.parent_dir_id = current_directory;

    if (!HavePermission(current_directory, active_user, 2)){
        std::string err_message = "Error(GetContent): permission denied";
        throw tmn_exception::RuntimeException(err_message);
    }

    for (auto& inner_file : files[current_directory].inner_files){
        if (fd.filename == files[inner_file].filename){
            std::string err_message = "Error(AddFile): File with this name already exists in this directory";
            throw tmn_exception::RuntimeException(err_message);
        }
    }
    
    // if (!fd.is_dir){    
        // std::ios::binary: file is opened in binary mode
        // std::ios::ate: pointer is set to the end of the file when the file is opened
        // file.tellg(): the current position of the read pointer
        // file.seekg() и file.seekp(): set the position of the read and write pointer respectively
    //     std::fstream file(recording_files[fd.physical_file_id], std::ios::in | std::ios::out | std::ios::binary | std::ios::ate); // opened in binary mode for correct size

    //     if (!file.is_open()) {
    //         std::string err_message = "Error(AddFile): error opening a physical file: " + recording_files[fd.physical_file_id];
    //         throw tmn_exception::RuntimeException(err_message);
    //     }

    //     content = std::to_string(fd.fd_id) + content;  

    //     fd.content_offset = file.tellg();
    //     file.write(content.c_str(), content.length());
    //     fd.content_size = static_cast<unsigned long>(file.tellp()) - fd.content_offset;
    //     file.close();
    // }

    files[current_directory].inner_files.insert(fd.fd_id);
    files.insert({fd.fd_id, fd});
}

void VirtualFileSystem::RenameFile(const std::string& old_filename, const std::string& new_filename){
    for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == old_filename){
            if(files[inner_file].owner_user != active_user){
                std::string err_message = "Error(RenameFile): Active user '" + users_table[active_user].username + "' is not the owner of this file/dir: " + old_filename;
                throw tmn_exception::RuntimeException(err_message);
            }
            files[inner_file].filename = new_filename;
        }
    }

    std::string err_message = "Error(RenameFile): file" + old_filename + "not found in current directory";
    throw tmn_exception::RuntimeException(err_message);
}

void VirtualFileSystem::SetOwnerGroup(unsigned long fd_id, unsigned long group_id) {
    if (!files.contains(fd_id)){
        std::string err_message = "Error(SetOwnerGroup): file not found in current directory";
        throw tmn_exception::RuntimeException(err_message);
    }

    if (!groups_table.contains(group_id)){
        std::string err_message = "Error(SetOwnerGroup): group not found";
        throw tmn_exception::RuntimeException(err_message);
    }

    if(files[fd_id].owner_user != active_user){
        std::string err_message = "Error(SetOwnerGroup): Active user '" + users_table[active_user].username +"' is not the owner of this file";
        throw tmn_exception::RuntimeException(err_message);
    }

    files[fd_id].owner_group = group_id;
}

void VirtualFileSystem::RemoveFile(const std::string& filename) {
    for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == filename){
            if (!files[inner_file].is_dir){
                if(files[inner_file].owner_user != active_user){
                    std::string err_message = "Error(RemoveFile): Active user '" + users_table[active_user].username +"' is not the owner of this file: " + filename;
                    throw tmn_exception::RuntimeException(err_message);
                }
                files[current_directory].inner_files.erase(inner_file);
                files.erase(inner_file);
            }
            else{
                std::string err_message = "Error(RemoveFile): file" + filename + "is directory";
                throw tmn_exception::RuntimeException(err_message);
            }
        }
    }
    
    std::string err_message = "Error(RemoveFile): file" + filename + "not found in current directory";
    throw tmn_exception::RuntimeException(err_message);
}

void VirtualFileSystem::RemoveDir(const std::string& filename, bool is_recursive){
    for (auto& matched_file : files[current_directory].inner_files){
        // Iteration-searching for a directory
        if (files[matched_file].filename == filename){
            if (files[matched_file].is_dir){
                if(files[matched_file].owner_user != active_user){
                    std::string err_message = "Error(RemoveDir): Active user '" + users_table[active_user].username +"' is not the owner of this directory: " + filename;
                    throw tmn_exception::RuntimeException(err_message);
                }

                if (!is_recursive) {
                    // Iteration-check before deletion :
                    for (unsigned long inner_fd_id : files[matched_file].inner_files) {
                        if (files[inner_fd_id].is_dir && files[inner_fd_id].inner_files.empty()){
                            std::string err_message = 
                            "Error(RemoveFile): you cannot delete a directory with non-empty subdirectories: '" + 
                                filename + "/" + files[inner_fd_id].filename  + "'\n" +
                            "You can use flag 'recursive' to remove all internal content";

                            throw tmn_exception::RuntimeException(err_message);
                        }
                    }
                    // Iteration with removing files :
                    for (unsigned long inner_fd_id : files[matched_file].inner_files) {
                        files[matched_file].inner_files.erase(inner_fd_id);
                        files.erase(inner_fd_id);
                    }
                }
                else{
                    for (unsigned long inner_fd_id : files[matched_file].inner_files) {
                        if (files[inner_fd_id].is_dir && files[inner_fd_id].inner_files.empty() || !files[inner_fd_id].is_dir) {
                            files[matched_file].inner_files.erase(inner_fd_id);
                            files.erase(inner_fd_id);
                        }
                        else{
                            try {
                                RemoveDir(files[inner_fd_id].filename, true);
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
                std::string err_message = "Error(RemoveFile): file '" + filename + "' is not directory";
                throw tmn_exception::RuntimeException(err_message);
            }
        }
    }

    std::string err_message = "Error(RemoveFile): file '" + filename + "' not found in current directory";
    throw tmn_exception::RuntimeException(err_message);
}

std::string tmn_vfs::VirtualFileSystem::GetFileContent(const std::string& filename) {
    for (auto& inner_id : files[current_directory].inner_files){
        if (files[inner_id].filename == filename){
            
            if (!HavePermission(inner_id, active_user, 1)){
                std::string err_message = "Error(GetContent): permission denied";
                throw tmn_exception::RuntimeException(err_message);
            }

            if (!files[inner_id].is_dir){
                std::ifstream file(recording_files[files[inner_id].physical_file_id], std::ios::binary);

                if (!file.is_open()) {
                    std::string err_message = "Error(GetContent): could not open file (std::fstream)";
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
            std::string err_message = "Error(GetContent): file '" + filename + "' is not regular file";
            throw tmn_exception::RuntimeException(err_message);
        }
    }
    std::string err_message = "Error(GetContent): file '" + filename + "' not found in specified directory";
    throw tmn_exception::RuntimeException(err_message);
}

}