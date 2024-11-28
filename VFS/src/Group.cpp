#include <sstream>

#include "../include/Group.hpp"
#include "../include/Utils.hpp"

namespace tmn_vfs {

Group::Group(const Group& other) noexcept :
    group_id(other.group_id),
    groupname(other.groupname),
    user_id(other.user_id),
    creation_time(other.creation_time),
    members(other.members) {}

Group::Group(Group&& other) noexcept :
    group_id(other.group_id),
    groupname(std::move(other.groupname)),
    user_id(other.user_id),
    creation_time(std::move(other.creation_time)),
    members(std::move(other.members)) {
        other.groupname = "-";
        other.creation_time = "-";
        other.members.clear();
    }

Group& Group::operator=(const Group& other) noexcept {
    if (this != &other) {
        group_id = other.group_id;
        groupname = other.groupname;
        user_id = other.user_id;
        creation_time = other.creation_time;
        members = other.members;
    }
    return *this;
}

Group& Group::operator=(Group&& other) noexcept {
    if (this != &other) {
        group_id = other.group_id;
        groupname = std::move(other.groupname);
        user_id = other.user_id;
        creation_time = std::move(other.creation_time);
        members = std::move(other.members);
        other.groupname = "-";
        other.creation_time = "-";
        other.members.clear();
    }
    return *this;
}

tmn_vfs::Group::~Group(){
    members.clear();
}

Group::Group(unsigned int group_id, 
    const std::string &groupname, unsigned int user_id, std::string creation_time) noexcept :
    group_id(group_id), groupname(groupname), user_id(user_id), creation_time(creation_time) {}

Group::Group(unsigned int group_id, 
    const std::string &groupname, unsigned int user_id) noexcept :
    group_id(group_id), groupname(groupname), user_id(user_id), creation_time(GetTimeNow()) {}

std::string Group::toString() const noexcept {
    std::string group_string = "";
    group_string.reserve(64);
    group_string += "gid:"; group_string += std::to_string(group_id);
    group_string += " groupname:"; group_string += groupname;
    group_string += " uid:"; group_string += std::to_string(user_id);
    group_string += " cr_time:"; group_string += creation_time;

    group_string += " [";
    for (const auto& user_id : members) {
        group_string += std::to_string(user_id);
        group_string += ",";
    }
    group_string += "]";

    return group_string;
}

Group Group::fromString(const std::string& group_string) {
    Group group;
    std::stringstream ss(group_string);
    std::string segment;

    // gid
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    group.group_id = std::stoul(segment);

    // groupname
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    group.groupname = segment;

    // uid
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    group.user_id = std::stoul(segment);

    // cr_time
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    group.creation_time = segment;

    // members
    std::getline(ss, segment, '[');
    std::getline(ss, segment, ']');

    std::stringstream member_ss(segment);
    std::string member_id_str;
    while (std::getline(member_ss, member_id_str, ',')) {
        std::size_t pos = member_id_str.find_last_not_of(" \t\n\r");
        if (pos != std::string::npos) {
            group.members.insert(std::stoul(member_id_str.substr(0, pos + 1)));
        }
    }

    return group;
}

}