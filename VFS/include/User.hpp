#pragma once

#include <string>

#include "../../include/Associative/HashSet.hpp"

namespace tmn_vfs {

class VirtualFileSystem;
class View;

enum class UserStatus : unsigned int { 
    SUPER = 0, 
    LOCAL = 1, 
    SYSTEM = 2 
};

struct User {
private:
    unsigned int user_id = 0;
    std::string username = "-";
    std::string fullname = "-";
    std::string creation_time = "-";
    std::string password_hash = "-";
    
    UserStatus status = UserStatus::LOCAL;

    tmn_associative::HashSet<unsigned int> groups;
    
public:
    User() = default;
    User(const User&) noexcept;
    User(User&&) noexcept;
    User& operator=(const User&) noexcept;
    User& operator=(User&&) noexcept;
    ~User();

    User(unsigned int user_id, 
        const std::string& username, const std::string& fullname, 
        const std::string& creation_time, const std::string& password_hash, 
        UserStatus status = UserStatus::LOCAL) noexcept;

    User(unsigned int user_id, 
        const std::string& username, 
        const std::string& creation_time, const std::string& password_hash, 
        UserStatus status = UserStatus::LOCAL) noexcept; 

    std::string toString() const noexcept;
    static User fromString(const std::string& user_string);

    friend class VirtualFileSystem;
    friend class View;
};

}