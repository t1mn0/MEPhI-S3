#pragma once

#include <string>

#include "../Associative/HashSet.hpp"

namespace tmn_vfs{

class VirtualFileSystem;

class Group {
private:
    std::string groupname;
    std::string password_hash;
    std::string creation_time;
    
    tmn_associative::HashSet<std::string> members;

public:
    Group() = default; 
    Group(const std::string& groupname, const std::string& password_hash) noexcept;

    const std::string& GetGroupName() const;
    const std::string& GetCreationTime() const;
    const std::string& GetPasswordHash() const;
    
    bool IsMember(const std::string& username) const;

    friend class VirtualFileSystem;
};

}