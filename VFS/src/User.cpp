#include <sstream>

#include "../include/User.hpp"
#include "../include/Utils.hpp"

namespace tmn::vfs{

User::User(const User& other) noexcept :
    user_id(other.user_id),
    username(other.username),
    fullname(other.fullname),
    creation_time(other.creation_time),
    password_hash(other.password_hash),
    status(other.status),
    groups(other.groups) {}

User::User(User&& other) noexcept :
    user_id(other.user_id),
    username(std::move(other.username)),
    fullname(std::move(other.fullname)),
    creation_time(std::move(other.creation_time)),
    password_hash(std::move(other.password_hash)),
    status(other.status),
    groups(std::move(other.groups)) {
        other.username = "-";
        other.fullname = "-";
        other.creation_time = "-";
        other.password_hash = "-";
        other.groups.clear();
}

User& User::operator=(const User& other) noexcept {
    if (this != &other) {
        user_id = other.user_id;
        username = other.username;
        fullname = other.fullname;
        creation_time = other.creation_time;
        password_hash = other.password_hash;
        status = other.status;
        groups = other.groups;
    }
    return *this;
}

User& User::operator=(User&& other) noexcept {
    if (this != &other) {
        user_id = other.user_id;
        username = std::move(other.username);
        fullname = std::move(other.fullname);
        creation_time = std::move(other.creation_time);
        password_hash = std::move(other.password_hash);
        status = other.status;
        groups = std::move(other.groups);
        other.username = "-";
        other.fullname = "-";
        other.creation_time = "-";
        other.password_hash = "-";
        other.groups.clear();
    }
    return *this;
}

User::User(uint64_t user_id, 
    const std::string& username, const std::string& fullname, 
    const std::string& creation_time, const std::string& password_hash, 
    UserStatus status) noexcept 
    :
    user_id(user_id), username(username), fullname(fullname), 
    creation_time(creation_time), password_hash(password_hash), status(status) {}

User::User(uint64_t user_id, 
    const std::string& username,
    const std::string& creation_time, const std::string& password_hash, 
    UserStatus status) noexcept 
    :
    user_id(user_id), username(username), fullname(fullname), 
    creation_time(creation_time), password_hash(password_hash), status(status) {}

tmn::vfs::User::~User(){
    groups.clear();
}

std::string User::to_string() const noexcept {
    std::string user_string = "";
    user_string.reserve(64);
    user_string += "uid:"; user_string += std::to_string(user_id);
    user_string += " username:"; user_string += username;
    user_string += " fullname:"; user_string += fullname;
    user_string += " cr_time:"; user_string += creation_time;
    user_string += " password_hash:"; user_string += password_hash;
    user_string += " status:"; user_string += std::to_string(static_cast<uint64_t>(status));

    user_string += " [";
    for (const auto& group_id : groups) {
        user_string += std::to_string(group_id);
        user_string += ",";
    }
    user_string += "]";

    return user_string;
}

User User::from_string(const std::string& user_string) {
    User user;
    std::stringstream ss(user_string);
    std::string segment;

    // uid
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    user.user_id = std::stoul(segment);

    // username
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    user.username = segment;

    // fullname
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    user.fullname = segment;

    // cr_time
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    user.creation_time = segment;

    // password_hash
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    user.password_hash = segment;

    // status
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    user.status = static_cast<UserStatus>(std::stoi(segment));

    // groups
    std::getline(ss, segment, '[');
    std::getline(ss, segment, ']');

    std::stringstream group_ss(segment);
    std::string group_id_str;
    while (std::getline(group_ss, group_id_str, ',')) {
        std::size_t pos = group_id_str.find_last_not_of(" \t\n\r");
        if (pos != std::string::npos) {
            user.groups.insert(std::stoul(group_id_str.substr(0, pos + 1)));
        }
    }

    return user;
}

}