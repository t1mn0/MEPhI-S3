#include "../../include/VFS/Group.hpp"
#include "../../include/VFS/Utils.hpp"

namespace tmn_vfs {

Group::Group(const std::string& groupname, const std::string& owner_name) : 
    groupname(groupname), owner_user(owner_name), creation_time(GetTimeNow()) {}

const std::string& Group::GroupName() const noexcept { 
    return groupname; 
}

const std::string& Group::OwnerName() const noexcept { 
    return owner_user; 
}

const std::string& Group::CreationTime() const  noexcept { 
    return creation_time; 
}

bool Group::IsMember(const std::string& username) const noexcept {
    return members.contains(username);
}

nlohmann::json Group::to_json() const noexcept {
    nlohmann::json group_json;
    group_json["groupname"] = groupname;
    group_json["owner_user"] = owner_user;
    group_json["creation_time"] = creation_time;

    nlohmann::json members_json;
    for (const auto& member : members) {
        members_json.push_back(member);
    }
    group_json["members"] = members_json;

    return group_json;
}

}