#include "../../include/VFS/User.hpp"
#include "../../include/VFS/Utils.hpp"

namespace tmn_vfs{

User::User(const std::string& username, const std::string& fullname, const std::string& password_hash, UserStatus status) noexcept : 
    username(username), fullname(fullname), creation_time(GetTimeNow()),
    password_hash(password_hash), status(status) {
        groups.insert(username);
    }

const std::string& User::UserName() const { return username; }
const std::string& User::FullName() const { return fullname; }
const std::string& User::CreationTime() const { return creation_time; }
const UserStatus User::Status() const { return status; }

nlohmann::json User::to_json() const noexcept {
    nlohmann::json user_json;
    user_json["username"] = username;
    user_json["fullname"] = fullname;
    user_json["creation_time"] = creation_time;
    user_json["password_hash"] = password_hash;
    user_json["status"] = static_cast<int>(status);

    nlohmann::json groups_json;
    for (const auto& group : groups) {
        groups_json.push_back(group);
    }
    user_json["groups"] = groups_json;

    return user_json;
}

}