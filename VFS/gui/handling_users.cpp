#include <iostream>
#include <string>

#include "View.hpp"
#include "../include/Utils.hpp"

namespace tmn_vfs {

void View::users(bool v) const noexcept {
    if (!v) {
        for (auto& user : vfs.users_table){
            std::cout << user.second.username <<  std::endl;
        }
    }
    else{
        for (auto& user : vfs.users_table){
            std::cout << user.second.user_id << " " <<  user.second.username << 
            " " << user.second.fullname << " " << user.second.creation_time << " ";
            if (user.second.status == tmn_vfs::UserStatus::LOCAL) std::cout << "LOCAL" << std::endl;
            if (user.second.status == tmn_vfs::UserStatus::SUPER) std::cout << "SUPER" << std::endl;
            if (user.second.status == tmn_vfs::UserStatus::SYSTEM) std::cout << "SYSTEM" << std::endl;
        }
    }
}

void View::whoami(bool v) const noexcept {
    if (!v) {
        std::cout << vfs.users_table[vfs.active_user].username << std::endl;
    }
    else{
        std::cout << vfs.active_user << " " <<  vfs.users_table[vfs.active_user].username << 
        " " << vfs.users_table[vfs.active_user].fullname << " " << vfs.users_table[vfs.active_user].creation_time << " ";
        if (vfs.users_table[vfs.active_user].status == tmn_vfs::UserStatus::LOCAL) std::cout << "LOCAL" << std::endl;
        if (vfs.users_table[vfs.active_user].status == tmn_vfs::UserStatus::SUPER) std::cout << "SUPER" << std::endl;
        if (vfs.users_table[vfs.active_user].status == tmn_vfs::UserStatus::SYSTEM) std::cout << "SYSTEM" << std::endl;
    }
}

void View::usercount() const noexcept {
    std::cout << vfs.CountUser() << std::endl;
}

void View::login(const std::string& username, const std::string& password_hash) noexcept {
    if (vfs.Authorization(username, password_hash)){
        std::cout << "Successful authorization" << std::endl;
    }
}

void View::newuser(const std::string& username, const std::string& fullname, const std::string& password_hash, bool st) noexcept {
    if (IsGoodUserName(username)){
        if (IsGoodUserFullName(fullname)){
            User user(vfs.users_table.size() + 1, username,  fullname, tmn_vfs::GetTimeNow(), password_hash);
            vfs.AddUser(user);
            if (!st){
                vfs.Authorization(username, password_hash);
            }
        }
        else{
            std::cout << "Fullname is invalid" << std::endl;
        }
    }
    else{
        std::cout << "Username is invalid" << std::endl;
    }
}

}