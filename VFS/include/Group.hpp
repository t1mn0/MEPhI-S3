#pragma once

#include <string>
#include <cstdint>

#include "../../include/Associative/HashSet.hpp"

namespace tmn_vfs {

class VirtualFileSystem;
class View;

struct Group {
private:
    uint64_t group_id = 0;
    std::string groupname = "-";
    uint64_t user_id = 0;
    std::string creation_time = "-";
    
    tmn_associative::HashSet<uint64_t> members;

public:
    Group() = default;
    Group(const Group&) noexcept;
    Group(Group&&) noexcept;
    Group& operator=(const Group&) noexcept;
    Group& operator=(Group&&) noexcept;
    ~Group();

    Group(uint64_t group_id, 
        const std::string& groupname, uint64_t user_id, 
        std::string creation_time) noexcept;

    Group(uint64_t group_id, 
        const std::string& groupname, uint64_t user_id) noexcept;

    std::string to_string() const noexcept;
    static Group from_string(const std::string& group_string);

    friend class VirtualFileSystem;
    friend class View;
};

}