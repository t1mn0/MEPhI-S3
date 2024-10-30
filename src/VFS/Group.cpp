#include "../../include/VFS/Group.hpp"
#include "../../include/VFS/Utils.hpp"
#include "Group.hpp"

namespace tmn_vfs {

Group::Group(const std::string& groupname, const std::string& password_hash) noexcept : 
    groupname(groupname), password_hash(password_hash), creation_time(GetTimeNow()) {}

const std::string& Group::GetGroupName() const { return groupname; }
const std::string& Group::GetCreationTime() const { return creation_time; }

bool Group::IsMember(const std::string& username) const {
    return members.contains(username);
}

 const std::string& Group::GetPasswordHash() const {        
    return password_hash;
}

};