#include <string>

#include "../include/VFS.hpp"
#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn_vfs {

// AddGroup(group) not added active_user to argument-group
void VirtualFileSystem::AddGroup(Group& group){
    if(groupnames.contains(group.groupname) || groups_table.contains(group.group_id)){
        std::string err_message = "Error(AddGroup): Group '" + group.groupname + std::to_string(group.group_id) + std::to_string(group_id) + "' is already in system";
        throw tmn_exception::RuntimeException(err_message);
    }

    users_table[group.user_id].groups.insert(group.group_id);
    groups_table.insert(tmn::Pair<const unsigned long, Group>(group.group_id, group));
    groupnames.insert(tmn::Pair<const std::string, unsigned long>(group.groupname, group.group_id));
}

void VirtualFileSystem::AddUserToGroup(const std::string& username, const std::string& groupname) {
    if(!usernames.contains(username)){
        std::string err_message = "Error(AddUserToGroup): User not found: " + username;
        throw tmn_exception::RuntimeException(err_message);
    }

    if(!groupnames.contains(groupname)){
        std::string err_message = "Error(AddUserToGroup): Group not found: " + groupname;
        throw tmn_exception::RuntimeException(err_message);
    }

    if(groups_table[groupnames[groupname]].user_id != active_user){
        std::string err_message = "Error(AddUserToGroup): Only the creator of the group can add to the group";
        throw tmn_exception::RuntimeException(err_message);
    }

    if(users_table[usernames[username]].groups.contains(groupnames[groupname])){
        std::string err_message = "Error(AddUserToGroup): User with name '" + username + "' already is in group: '" + groupname + "'";
        throw tmn_exception::RuntimeException(err_message);
    }

    users_table[usernames[username]].groups.insert(groupnames[groupname]);
    groups_table[groupnames[groupname]].members.insert(usernames[username]);
}

void VirtualFileSystem::RemoveUserFromGroup(const std::string& username, const std::string& groupname) {
    if(!usernames.contains(username)){
        std::string err_message = "Error(AddUserToGroup): User not found: " + username;
        throw tmn_exception::RuntimeException(err_message);
    }

    if(!groupnames.contains(groupname)){
        std::string err_message = "Error(RemoveUserFromGroup): Group not found: " + groupname;
        throw tmn_exception::RuntimeException(err_message);
    }

    if(!users_table[usernames[username]].groups.contains(groupnames[groupname])){
        std::string err_message = "Error(RemoveUserFromGroup): User with name '" + username + "' is not in group: '" + groupname + "'";
        throw tmn_exception::RuntimeException(err_message);
    }

    if(username == groupname){
        std::string err_message = "Error(RemoveUserFromGroup): Cannot remove user from their default group";
        throw tmn_exception::RuntimeException(err_message);
    }

    users_table[usernames[username]].groups.erase(groupnames[groupname]);
    groups_table[groupnames[groupname]].members.erase(usernames[username]);
}

void VirtualFileSystem::RemoveGroup(const std::string& groupname){
    if(!groupnames.contains(groupname)){
        std::string err_message = "Error(RemoveGroup): Group not found: " + groupname;
        throw tmn_exception::RuntimeException(err_message);
    }

    if (usernames.contains(groupname)){
        std::string err_message = "Error(RemoveGroup): Cannot delete user's default group: " + groupname;
        throw tmn_exception::RuntimeException(err_message);
    }

    if(groups_table[groupnames[groupname]].user_id != active_user){
        std::string err_message = "Error(RemoveGroup): Group can be deleted only by its creator";
        throw tmn_exception::RuntimeException(err_message);
    }

    for (const auto& user_id : groups_table[groupnames[groupname]].members){
        users_table[user_id].groups.erase(groupnames[groupname]);
    }   

    groups_table.erase(groupnames[groupname]);
    groupnames.erase(groupname);
}

}