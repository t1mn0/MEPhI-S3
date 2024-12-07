#include <iostream>
#include <string>

#include "View.hpp"
#include "../include/Utils.hpp"
#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn_vfs {

void View::groups(bool v) const noexcept {
    if (!v) {
        for (auto& group : vfs.groups_table){
            std::cout << group.second.groupname <<  std::endl;
        }
    }
    else{
        for (auto& group : vfs.groups_table){
            std::cout << group.second.group_id << " " <<  group.second.groupname << 
            " " << group.second.user_id << " " << group.second.creation_time << std::endl;
        }
    }
}

void View::mygroups(bool v) const noexcept {
    if (!v) {
        for (auto& group : vfs.users_table[vfs.active_user].groups){
            std::cout << vfs.groups_table[group].groupname <<  std::endl;
        }
    }
    else{
        for (auto& group : vfs.users_table[vfs.active_user].groups){
            std::cout << vfs.groups_table[group].group_id << " " <<  vfs.groups_table[group].groupname << 
            " " << vfs.groups_table[group].user_id<< " " << vfs.groups_table[group].creation_time << std::endl;
        }
    }
}

void View::groupcount() const noexcept {
    std::cout << vfs.count_groups() << std::endl;
}

void View::newgroup(const std::string& groupname) noexcept {
    if (is_good_username(groupname)){
        Group group(1001 + vfs.group_id, groupname, vfs.active_user, get_time_now());
        group.members.insert(vfs.active_user);
        try{
            vfs.add_group(group);
            ++vfs.group_id;
        }
        catch (tmn_exception::RuntimeException& e){
            std::cerr << e.what() << std::endl;
            return;
        }
        
    }
    else{
        std::cerr << "Groupname is invalid" << std::endl;
    }
}

void View::addtogroup(const std::string& username, const std::string& groupname) noexcept {
    try{
        vfs.add_user_to_group(username, groupname);
    }
    catch (tmn_exception::RuntimeException& e){
        std::cerr << e.what() << std::endl;
    }
}

void View::rmmefromgroup(const std::string& groupname) noexcept {
    try{
        vfs.remove_user_from_group(vfs.users_table[vfs.active_user].username, groupname);
    }
    catch (tmn_exception::RuntimeException& e){
        std::cerr << e.what() << std::endl;
    }
}

void View::rmgroup(const std::string& groupname) noexcept {
    try{
        vfs.remove_group(groupname);
    }
    catch (tmn_exception::RuntimeException& e){
        std::cerr << e.what() << std::endl;
    }
}

}