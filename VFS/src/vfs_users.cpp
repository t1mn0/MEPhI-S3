#include <string>
#include <iostream> // for std::cerr

#include "../include/VFS.hpp"

namespace tmn_vfs {

bool VirtualFileSystem::AddUser(User& user) {
    if(groupnames.contains(user.username)){
        std::cerr << "Error(AddUser): Cannot create default group for that user: group '"<< user.username <<"' already is in system" << std::endl;
        return false;
    }
    if(users_table.contains(user.user_id)){
        std::cerr << "Error(AddUser): User with name '"<< user.username <<"' already is in system" << std::endl;
        return false;
    }
    Group default_group(user.user_id, user.username, user.user_id, user.creation_time);
    default_group.members.insert(user.user_id);
    if (AddGroup(default_group)){
        user.groups.insert(user.user_id);
        users_table.insert(tmn::Pair<const unsigned long, User>(user.user_id, user));
        usernames.insert(tmn::Pair<const std::string, unsigned long>(user.username, user.user_id));
        return true;
    }
    return false;
}

bool VirtualFileSystem::Authorization(const std::string& username, const std::string& password_hash) {
    if(!usernames.contains(username)){
        std::cerr << "Error(Authorization): User with name '"<< username <<"' is not in system" << std::endl;
    }
    else{
        if(users_table[usernames[username]].password_hash == password_hash){
            active_user = users_table[usernames[username]].user_id;
            return true;
        }
        else{
            std::cerr << "Error(Authorization): Wrong password for user : " << username << std::endl;
        }
    }
    return false;
}

bool VirtualFileSystem::RemoveUser(const std::string& username){
    if(!usernames.contains(username)){
        std::cerr << "Error(RemoveUser): User not found: " << username << std::endl;
        return false;
    }
    else if (usernames[username] == active_user){
        std::cerr << "Error(RemoveUser): User is active: " << username << std::endl;
        return false;
    }

    for (const auto& group_id : users_table[usernames[username]].groups){
        groups_table[group_id].members.erase(usernames[username]);
    }   
    users_table.erase(usernames[username]);
    usernames.erase(username);
    return true;
}

}