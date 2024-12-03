#include <iostream>
#include <string>

#include "View.hpp"
#include "../include/Utils.hpp"
#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn_vfs {

void View::mkdir(const std::string& dirname, std::string path) noexcept {
    if(!IsGoodFileName(dirname)){
        std::cerr << "Bad name for directory" << std::endl;
        return;
    }

    unsigned long curdir = vfs.current_directory; // for quick return to the directory from which the command was executed

    ++vfs.fd_id;

    if (path.empty()){

        FileDescriptor fd(vfs.fd_id, true, 0, vfs.current_directory, dirname, 0, 0, vfs.active_user);
        
        try {
            vfs.AddFile(fd);
        }
        catch (tmn_exception::RuntimeException& e){
            --vfs.fd_id;

            std::cerr << e.what() << std::endl;
        }

        return;
    }

    try {
        vfs.GoTo(path);
    }
    catch (tmn_exception::RuntimeException& e){
        --vfs.fd_id;
        
        std::cerr << e.what() << std::endl;
        return;
    }
        
    FileDescriptor fd(vfs.fd_id, true, 0, vfs.current_directory, dirname, 0, 0, vfs.active_user);

    try {
        vfs.AddFile(fd);
    }
    catch (tmn_exception::RuntimeException& e){
        --vfs.fd_id;

        std::cerr << e.what() << std::endl;
        return;
    }

    vfs.current_directory = curdir; // quick return to the directory from which the command was executed
}

void View::mkfile(const std::string& filename, std::filesystem::path ph_path, std::string path) noexcept {
    if(!IsGoodFileName(filename)){
        std::cerr << "Bad name for file" << std::endl;
        return;
    }

    unsigned long curdir = vfs.current_directory;

    ++vfs.fd_id;

    unsigned long rec_id = 0;
    bool flag = false;
    for (auto& pair : vfs.recording_files){
        if (pair.second == ph_path.string()){
            rec_id = pair.first;
            flag = true;
            break;
        }
    }
    
    if (!flag) {
        ++vfs.rec_id;
        rec_id = vfs.rec_id;
        vfs.recording_files.insert({rec_id, ph_path.string()});
    }

    ++vfs.fd_id;

    if (path.empty() || path == "-c"){

        FileDescriptor fd(vfs.fd_id, false, rec_id, vfs.current_directory, filename, 0, 0, vfs.active_user);
        
        try {
            vfs.AddFile(fd);
        }
        catch (tmn_exception::RuntimeException& e){
            if (!flag) {
                vfs.recording_files.erase(rec_id);
            }

            --vfs.fd_id;

            std::cerr << e.what() << std::endl;
        }

        return;
    }

    try {
        vfs.GoTo(path);
    }
    catch (tmn_exception::RuntimeException& e){
        if (!flag) {
            vfs.recording_files.erase(rec_id);
        }

        --vfs.fd_id;

        std::cerr << e.what() << std::endl;
        return;
    }
        
    FileDescriptor fd(vfs.fd_id, false, rec_id, vfs.current_directory, filename, 0, 0, vfs.active_user);

    try {
        vfs.AddFile(fd);
    }
    catch (tmn_exception::RuntimeException& e){
        if (!flag) {
            vfs.recording_files.erase(rec_id);
        }

        --vfs.fd_id;

        std::cerr << e.what() << std::endl;
    }

    vfs.current_directory = curdir;
    return;
}

void View::setgroup(const std::string &filename, const std::string& groupname) noexcept {
    if (!vfs.groupnames.contains(groupname)){
        std::cerr << "Group not found: '" + groupname + "'" << std::endl;
        return;
    }
    for (auto& inner_id : vfs.files[vfs.current_directory].inner_files){
        if (!vfs.files.contains(inner_id)){
            std::cerr << "File not found: '" + filename + "'" << std::endl;
        return;
        }
        
        try {
            vfs.SetOwnerGroup(inner_id, vfs.groupnames[groupname]);
        }
        catch (tmn_exception::RuntimeException& e){
            std::cerr << e.what() << std::endl;
        }
    }
}

void View::renamefile(const std::string& old_filename, const std::string& new_filename) noexcept {
    if(!IsGoodFileName(new_filename)){
        std::cerr << "Bad name for directory" << std::endl;
        return;
    }

    try {
        vfs.RenameFile(old_filename, new_filename);
    }
    catch (tmn_exception::RuntimeException& e){
        std::cerr << e.what() << std::endl;
    }
}

void View::cat(const std::string& filename, std::string path) noexcept {
    unsigned long curdir = vfs.current_directory;

    if (!path.empty()){
        try{
            vfs.GoTo(path);
        }
        catch (tmn_exception::RuntimeException& e){
            std::cerr << e.what() << std::endl;
            return;
        }
    }

    std::string content = "";
    
    try{
        content = vfs.GetFileContent(filename);
    }
    catch (tmn_exception::RuntimeException& e){
        std::cerr << e.what() << std::endl;
        return;
    }
    if (content.empty()){
        std::cout <<  "Content of the selected file is empty" << std::endl;
    }
    else{
        std::cout << content << std::endl;
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
    try{
        vfs.GoTo(path);
    }
    catch (tmn_exception::RuntimeException& e){
        std::cerr << e.what() << std::endl;
    }
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