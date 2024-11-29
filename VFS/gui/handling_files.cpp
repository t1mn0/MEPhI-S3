#include <iostream>
#include <string>

#include "View.hpp"
#include "../include/Utils.hpp"

namespace tmn_vfs {

void View::mkdir(const std::string& dirname, std::string&& path) noexcept {
    if (path.empty()){
        FileDescriptor fd(vfs.files.size(), true, 
            vfs.NextRecordFile(), vfs.current_directory, dirname, 0, 0, vfs.active_user);
        vfs.AddFile(fd);
    }
    else{
        unsigned long curdir = vfs.current_directory;
        if(vfs.GoTo(path)){
            FileDescriptor fd(vfs.files.size(), true, 
                vfs.NextRecordFile(), vfs.current_directory, dirname, 0, 0, vfs.active_user);
            vfs.AddFile(fd);
        }
        vfs.current_directory = curdir;
    }
}

void View::mkfile(const std::string& filename, std::string&& content, std::string path) noexcept {
    unsigned long curdir = vfs.current_directory;
    if (!path.empty()){
        std::cout << path.size() << std::endl;
        if(!vfs.GoTo(path)){
            return;
        }
    }

    FileDescriptor fd(vfs.files.size() + 1, false, 
                vfs.NextRecordFile(), vfs.current_directory, filename, 0, 0, vfs.active_user);
    vfs.AddFile(fd, content);
    vfs.current_directory = curdir;
}

// void View::rmdir(const std::string& dirname, std::string&& path, bool r) noexcept {
//     // TODO
// }

// void View::rmfile(const std::string& filename, std::string&& path) noexcept {
//     // TODO
// }

void View::cat(const std::string& filename, std::string&& path) noexcept {
    unsigned long curdir = vfs.current_directory;
    if (!path.empty()){
        if(!vfs.GoTo(path)){
            return;
        }
    }
    auto content = vfs.GetContent(filename);
    if (content.empty()){
        std::cout <<  "Content of the selected file is empty" << std::endl;
    }
    else{
        std::cout <<  content << std::endl;
    }
    vfs.current_directory = curdir;
}

void View::ls(bool v) noexcept {
    if (v){
        for (auto& id : vfs.files[vfs.current_directory].inner_files){
            if (vfs.current_directory != id){ // for root-dir (parent dir = 0) 
                if (vfs.files[id].is_dir){
                    std::cout << "[DIR]" << vfs.files[id].file_permissions.toString() << " " << 
                    vfs.files[id].creation_time << " " << 
                    vfs.files[id].filename << std::endl;
                }
                else{
                    std::cout << "[REG]" << vfs.files[id].file_permissions.toString() << " " << 
                    vfs.files[id].content_size << " bytes " << vfs.files[id].creation_time << " " << 
                    vfs.files[id].filename << std::endl;
                }
            }
        }
    }
    else{
        for (auto& id : vfs.files[vfs.current_directory].inner_files){
            if (vfs.current_directory != id){
                std::cout << vfs.files[id].filename << std::endl;
            }
        }
    }
}

void View::cd(std::string& path) noexcept {
    vfs.GoTo(path);
}

void View::find(short filetype, bool where, const std::string& name_pattern) noexcept {
    tmn_sequence::ArraySequence<unsigned long> result = vfs.FindFileByName(name_pattern, where);

    if (result.empty()){
        std::cout << "No files found" << std::endl;
    } 
    else{
        if (filetype == 0){
            std::cout << "[SEARCH RESULTS] : " << std::endl;
            for (auto& id : result){
                if (vfs.files[id].is_dir){
                    std::cout << "  - [DIR] " << vfs.DoPath(id).value() << std::endl;
                }
                else {
                    std::cout << "  - [REG] " << vfs.DoPath(id).value() << std::endl;
                }
            }
        }
        else if (filetype == 1){
            bool just_one = false;
            std::cout << "[SEARCH RESULTS] : " << std::endl;
            for (auto& id : result){
                if (!vfs.files[id].is_dir){
                    std::cout << "  - [REG] " << vfs.DoPath(id).value() << std::endl;
                    just_one = true;
                }
            }
            if (!just_one){
                std::cout << "No files found" << std::endl;
            }
        }
        else {
            bool just_one = false;
            std::cout << "[SEARCH RESULTS] : " << std::endl;
            for (auto& id : result){
                if (vfs.files[id].is_dir){
                    std::cout << "  - [DIR] " << vfs.DoPath(id).value() << std::endl;
                    just_one = true;
                }
            }
            if (!just_one){
                std::cout << "No files found" << std::endl;
            }
        }
    }
}

}