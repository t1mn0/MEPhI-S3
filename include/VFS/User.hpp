#pragma once

#include <string>
#include <unordered_set>
#include <memory>

namespace tmn_vfs{

class VirtualFileSystem;

enum class UserStatus { SUPER, LOCAL, GUEST };

class User {
private:
    const std::string username;
    const std::string fullname;
    const std::string creation_time;
    const std::string password_hash;
    
    const UserStatus status = UserStatus::GUEST;

    std::unordered_set<std::string> groups;
    
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

};