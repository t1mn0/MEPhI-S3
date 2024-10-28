#pragma once

#include <string>
#include <unordered_set>
#include <memory>

namespace tmn_vfs{

class VirtualFileSystem;

class Group {
private:
    const std::string groupname;
    const std::string password_hash;
    const std::string creation_time;
    std::unordered_set<std::string> members;

public:
    Group() = default; 
    Group(const std::string& groupname, const std::string& password_hash) noexcept;

    const std::string& GetGroupName() const;
    const std::string& GetCreationTime() const;
    
    bool IsMember(const std::string& username) const;

    friend class VirtualFileSystem;
};

};