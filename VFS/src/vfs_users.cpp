#include <string>

#include "../include/VFS.hpp"
#include "../../include/Exceptions/RuntimeException.hpp"

namespace tmn_vfs {

void VirtualFileSystem::AddUser(User& user) {
    if(user_id == 1000){
        std::string err_message = "Error(AddUser): VFS already contains the maximum number of users";
        throw tmn_exception::RuntimeException(err_message);
    }

    if(groupnames.contains(user.username)){
        std::string err_message = "Error(AddUser): Cannot create default group for that user: group '" + user.username + "' already is in system";
        throw tmn_exception::RuntimeException(err_message);
    }

    if(users_table.contains(user.user_id)){
        std::string err_message = "Error(AddUser): User with name '" + user.username + "' already is in system";
        throw tmn_exception::RuntimeException(err_message);
    }

    // When a user is added, his or her group automatically appears in the system
    Group default_group(user.user_id, user.username, user.user_id, user.creation_time);
    default_group.members.insert(user.user_id);

    try {
        AddGroup(default_group);
        user.groups.insert(user.user_id);
        users_table.insert(tmn::Pair<const unsigned long, User>(user.user_id, user));
        usernames.insert(tmn::Pair<const std::string, unsigned long>(user.username, user.user_id));
    }
    catch (tmn_exception::RuntimeException& e){
        throw e;
    }
}

void VirtualFileSystem::Authorization(const std::string& username, const std::string& password_hash) {
    if(!usernames.contains(username)){
        std::string err_message = "Error(Authorization): User with name '" + username + "' is not in system";
        throw tmn_exception::RuntimeException(err_message);
    }
    
    if(users_table[usernames[username]].password_hash != password_hash){
        std::string err_message = "Error(Authorization): Wrong password for user : " + username;
        throw tmn_exception::RuntimeException(err_message);
    }

    active_user = users_table[usernames[username]].user_id;
}

void VirtualFileSystem::RemoveUser(const std::string& username){
    if(!usernames.contains(username)){
        std::string err_message = "Error(RemoveUser): User not found: " + username;
        throw tmn_exception::RuntimeException(err_message);
    }
    else if (usernames[username] == active_user){
        std::string err_message = "Error(RemoveUser): User is active: " + username;
        throw tmn_exception::RuntimeException(err_message);
    }

    // Removing group from the group lists of all members of this group
    for (const auto& group_id : users_table[usernames[username]].groups){
        groups_table[group_id].members.erase(usernames[username]);
    }   

    users_table.erase(usernames[username]);
    usernames.erase(username);
}

}