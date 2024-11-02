#pragma once

#include <string>

#include "../Associative/HashSet.hpp"

namespace tmn_vfs{

class VirtualFileSystem;

enum class UserStatus { SUPER, LOCAL, GUEST };

class User {
private:
    std::string username;
    std::string fullname;
    std::string creation_time;
    std::string password_hash;
    
    UserStatus status = UserStatus::GUEST;

    tmn_associative::HashSet<std::string> groups;
    
public:
    User() = default;
    User(const std::string& username, const std::string& fullname, const std::string& password_hash, UserStatus status) noexcept;

    const std::string& GetUserName() const;
    const std::string& GetFullName() const;
    const std::string& GetCreationTime() const;
    const std::string& GetPasswordHash() const;
    const UserStatus GetStatus() const;

    bool IsInGroup(const std::string& groupname) const;

    friend class VirtualFileSystem;
};

}