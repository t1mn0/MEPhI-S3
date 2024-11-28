#include <string>
#include <iostream> // for std::cerr

#include "../include/VFS.hpp"

namespace tmn_vfs {

bool VirtualFileSystem::AddGroup(Group& group){
    if(groups_table.contains(group.group_id)){
        std::cerr << "Error(AddGroup): Group '"<< group.groupname <<"' is already in system" << std::endl;
        return false;
    }
    group.members.insert(active_user);
    groups_table.insert(tmn::Pair<const unsigned long, Group>(group.group_id, group));
    groupnames.insert(tmn::Pair<const std::string, unsigned long>(group.groupname, group.group_id));
    return true;
}

bool VirtualFileSystem::AddUserToGroup(const std::string& username, const std::string& groupname) {
    if(!usernames.contains(username)){
        std::cerr << "Error(AddUserToGroup): User not found: " << username << std::endl;
        return false;
    }
    if(!groupnames.contains(groupname)){
        std::cerr << "Error(AddUserToGroup): Group not found: " << groupname << std::endl;
        return false;
    }
    if(groups_table[groupnames[groupname]].user_id != active_user){
        std::cerr << "Error(AddUserToGroup): Only the creator of the group can add to the group" << std::endl;
        return false;
    }
    if(users_table[usernames[username]].groups.contains(groupnames[groupname])){
        std::cerr << "Error(AddUserToGroup): User with name '"<< username <<"' already is in group: '" << groupname << "'" << std::endl;
        return false;
    }
    users_table[usernames[username]].groups.insert(groupnames[groupname]);
    groups_table[groupnames[groupname]].members.insert(usernames[username]);
    return true;
}

bool VirtualFileSystem::RemoveUserFromGroup(const std::string& username, const std::string& groupname) {
    if(!usernames.contains(username)){
        std::cerr << "Error(AddUserToGroup): User not found: " << username << std::endl;
        return false;
    }
    if(!groupnames.contains(groupname)){
        std::cerr << "Error(RemoveUserFromGroup): Group not found: " << groupname << std::endl;
        return false;
    }
    if(!users_table[usernames[username]].groups.contains(groupnames[groupname])){
        std::cerr << "Error(RemoveUserFromGroup): User with name '"<< username <<"' is not in group: '" << groupname << "'" << std::endl;
        return false;
    }
    if(username == groupname){
        std::cerr << "Error(RemoveUserFromGroup): Cannot remove user from their default group" << std::endl;
        return false;
    }
    users_table[usernames[username]].groups.erase(groupnames[groupname]);
    groups_table[groupnames[groupname]].members.erase(usernames[username]);
    return true;
}

bool VirtualFileSystem::RemoveGroup(const std::string& groupname){
    if(!groupnames.contains(groupname)){
        std::cerr << "Error(RemoveGroup): Group not found: " << groupname << std::endl;
        return false;
    }
    if (usernames.contains(groupname)){
        std::cerr << "Error(RemoveGroup): Cannot delete user's default group: " << groupname << std::endl;
        return false;
    }
    if(groups_table[groupnames[groupname]].user_id != active_user){
        std::cerr << "Error(RemoveGroup): Group can be deleted only by its creator" << std::endl;
        return false;
    }

    for (const auto& user_id : groups_table[groupnames[groupname]].members){
        users_table[user_id].groups.erase(groupnames[groupname]);
    }   
    groups_table.erase(groupnames[groupname]);
    groupnames.erase(groupname);
    return true;
}

}