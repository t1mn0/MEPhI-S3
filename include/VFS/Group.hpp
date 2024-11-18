#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "../Associative/HashSet.hpp"

namespace tmn_vfs{

class VirtualFileSystem;

class Group {
private:
    std::string groupname = "plug";
    std::string owner_user = "plug";
    std::string creation_time = "plug";
    
    tmn_associative::HashSet<std::string> members;

public:
    Group() = default; 
    Group(const std::string& groupname, const std::string& creater_name);

    const std::string& GroupName() const noexcept;
    const std::string& OwnerName() const noexcept;
    const std::string& CreationTime() const noexcept;
    
    bool IsMember(const std::string& username) const noexcept;

    nlohmann::json to_json() const noexcept;

    friend class VirtualFileSystem;
};

}