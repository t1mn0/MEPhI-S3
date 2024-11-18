#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "../Associative/HashSet.hpp"

namespace tmn_vfs{

class VirtualFileSystem;

enum class UserStatus { SUPER, LOCAL, SYSTEM };

class User {
private:
    std::string username;
    std::string fullname;
    std::string creation_time;
    std::string password_hash;
    
    UserStatus status = UserStatus::LOCAL;

    tmn_associative::HashSet<std::string> groups;
    
public:
    User() = default;
    User(const std::string& username, const std::string& fullname, const std::string& password_hash, UserStatus status) noexcept;

    const std::string& UserName() const;
    const std::string& FullName() const;
    const std::string& CreationTime() const;
    const UserStatus Status() const;

    nlohmann::json to_json() const noexcept;

    friend class VirtualFileSystem;
};

}