#include <iostream>
#include <string>

#include "View.hpp"
#include "../include/Utils.hpp"

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
    std::cout << vfs.CountGroup() << std::endl;
}

void View::newgroup(const std::string& groupname) noexcept {
    if (IsGoodUserName(groupname)){
        Group group(1001 + vfs.groups_table.size(), groupname, vfs.active_user, GetTimeNow());
        vfs.AddGroup(group);
    }
    else{
        std::cout << "Groupname is invalid" << std::endl;
    }
}

void View::addtogroup(const std::string& username, const std::string& groupname) noexcept {
    vfs.AddUserToGroup(username, groupname);
}

void View::rmmefromgroup(const std::string& groupname) noexcept {
    vfs.RemoveUserFromGroup(vfs.users_table[vfs.active_user].username, groupname);
}

void View::rmgroup(const std::string& groupname) noexcept {
    vfs.RemoveGroup(groupname);
}

}