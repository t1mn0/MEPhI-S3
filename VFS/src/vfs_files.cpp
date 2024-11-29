#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <iostream>

#include "../include/VFS.hpp"
#include "../include/Utils.hpp"

namespace tmn_vfs {

tmn_associative::HashSet<std::string> VirtualFileSystem::CurrentDirContent() {
    tmn_associative::HashSet<std::string> set;
    for (auto& fid : files[current_directory].inner_files){
        set.insert(files[fid].filename);
    }
    return set;
}

bool VirtualFileSystem::AddFile(FileDescriptor& fd, const std::string& content) {
    for (auto& inner_file : files[current_directory].inner_files){
        if (fd.filename == files[inner_file].filename){
            std::cerr << "Error(AddFile): File with this name already exists in this directory" << std::endl;
            return false;
        }
    }
    if (!fd.is_dir){
        std::fstream file(recording_files[fd.physical_file_id], std::ios::in | std::ios::out | std::ios::binary | std::ios::ate); // opened in binary mode for correct size

        if (!file.is_open()) {
            std::cerr << "Error(AddFile): error opening a file: " << recording_files[fd.physical_file_id] << std::endl;
            return false;
        }

        fd.content_offset = file.tellg();
        file.write(content.c_str(), content.length());
        fd.content_size = static_cast<unsigned long>(file.tellp()) - fd.content_offset;
        file.close();

        // Поиск конца файла в байтах:
        //std::ifstream f(recording_files[fd.physical_file_id], std::ios::binary);
        //f.seekg(0, std::ios::end);
        //std::streampos end_pos = f.tellg();

        //std::cout << "Позиция конца файла: " << end_pos << " байт" << std::endl;
        //f.close();
    }
    files[current_directory].inner_files.insert(fd.fd_id);
    files.insert({fd.fd_id, fd});
    return true;
}

bool VirtualFileSystem::RenameFile(const std::string& old_filename, const std::string& new_filename){
    for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == old_filename){
            files[inner_file].filename = new_filename;
            return true;
        }
    }
    std::cerr << "Error(RenameFile): file" << old_filename << "not found in current directory" << std::endl;
    return false;
}

bool VirtualFileSystem::RemoveFile(const std::string& filename) {
    for (auto& inner_file : files[current_directory].inner_files){
        if (files[inner_file].filename == filename){
            if (!files[inner_file].is_dir){
                files[current_directory].inner_files.erase(inner_file);
                files.erase(inner_file);
                return true;
            }
            else{
                std::cerr << "Error(RemoveFile): file" << filename << "is directory" << std::endl;
                return false;
            }
        }
    }
    std::cerr << "Error(RemoveFile): file" << filename << "not found in current directory" << std::endl;
    return false;
}

bool VirtualFileSystem::RemoveDir(const std::string& filename, bool is_recursive){
    for (auto& matched_file : files[current_directory].inner_files){
        // Iteration-searching for a directory
        if (files[matched_file].filename == filename){
            if (files[matched_file].is_dir){
                if (!is_recursive) {
                    // Iteration-check before deletion :
                    for (unsigned long inner_fd_id : files[matched_file].inner_files) {
                        if (files[inner_fd_id].is_dir && files[inner_fd_id].inner_files.empty()){
                            std::cerr << "Error(RemoveFile): you cannot delete a directory with non-empty subdirectories: " << 
                            filename << "/" << files[inner_fd_id].filename  << ";" << std::endl <<
                            "You can use flag 'recursive' to remove all internal content" << std::endl;
                            return false;
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
                            RemoveDir(files[inner_fd_id].filename, true);
                        }
                    }
                }
                
                files.erase(files[matched_file].parent_dir_id);
                files.erase(matched_file);

                return true;
            }
            else{
                std::cerr << "Error(RemoveFile): file" << filename << "is not directory" << std::endl;
                return false;
            }
        }
    }
    std::cerr << "Error(RemoveFile): file" << filename << "not found in current directory" << std::endl;
    return false;
}

std::string tmn_vfs::VirtualFileSystem::GetContent(const std::string& filename) {
    for (auto& inner_id : files[current_directory].inner_files){
        if (files[inner_id].filename == filename){
            if (!files[inner_id].is_dir){
                std::ifstream file(recording_files[files[inner_id].physical_file_id], std::ios::binary);

                if (!file.is_open()) {
                    throw std::runtime_error("Could not open file");
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
            std::cerr << "Error(GetContent): file" << filename << "is not regular file" << std::endl;
            return "";
        }
    }
    std::cerr << "Error(GetContent): file" << filename << "not found in specified directory" << std::endl;
    return "";
}

tmn_sequence::ArraySequence<unsigned long> VirtualFileSystem::FindFileByName(const std::string& filename, bool in_current_dir) {
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

}