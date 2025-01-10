#pragma once

#include <string>
#include <cstdint>

#include "../../include/Associative/HashSet.hpp"

namespace tmn {
namespace vfs {

class VirtualFileSystem;
class View;

enum class UserStatus : uint8_t { 
    SUPER = 0, 
    LOCAL = 1, 
    SYSTEM = 2 
};

struct User {
private:
    uint64_t user_id = 0;
    std::string username = "-";
    std::string fullname = "-";
    std::string creation_time = "-";
    std::string password_hash = "-";
    
    UserStatus status = UserStatus::LOCAL;

    tmn::associative::HashSet<uint64_t> groups;
    
public:
    User() = default;
    User(const User&) noexcept;
    User(User&&) noexcept;
    User& operator=(const User&) noexcept;
    User& operator=(User&&) noexcept;
    ~User();

    User(uint64_t user_id, 
        const std::string& username, const std::string& fullname, 
        const std::string& creation_time, const std::string& password_hash, 
        UserStatus status = UserStatus::LOCAL) noexcept;

    User(uint64_t user_id, 
        const std::string& username, 
        const std::string& creation_time, const std::string& password_hash, 
        UserStatus status = UserStatus::LOCAL) noexcept; 

    std::string to_string() const noexcept;
    static User from_string(const std::string& user_string);

    friend class VirtualFileSystem;
    friend class View;
};

}
}