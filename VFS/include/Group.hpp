#pragma once

#include <string>

#include "../../include/Associative/HashSet.hpp"

namespace tmn_vfs{

class VirtualFileSystem;
class View;

struct Group {
private:
    unsigned int group_id = 0;
    std::string groupname = "-";
    unsigned int user_id = 0;
    std::string creation_time = "-";
    
    tmn_associative::HashSet<unsigned int> members;

public:
    Group() = default;
    Group(const Group&) noexcept;
    Group(Group&&) noexcept;
    Group& operator=(const Group&) noexcept;
    Group& operator=(Group&&) noexcept;
    ~Group();

    Group(unsigned int group_id, 
        const std::string& groupname, unsigned int user_id, 
        std::string creation_time) noexcept;

    Group(unsigned int group_id, 
        const std::string& groupname, unsigned int user_id) noexcept;

    std::string toString() const noexcept;
    static Group fromString(const std::string& group_string);

    friend class VirtualFileSystem;
    friend class View;
};

}