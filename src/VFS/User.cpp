#include "../../include/VFS/User.hpp"
#include "../../include/VFS/Utils.hpp"

namespace tmn_vfs{

User::User(const std::string& username, const std::string& fullname, const std::string& password_hash, UserStatus status) noexcept : 
    username(username), fullname(fullname), creation_time(GetTimeNow()),
    password_hash(password_hash), status(status) {
        groups.insert("default");
    }

const std::string& User::GetUserName() const { return username; }
const std::string& User::GetFullName() const { return fullname; }
const std::string& User::GetCreationTime() const { return creation_time; }
const std::string& User::GetPasswordHash() const { return password_hash; }
const UserStatus User::GetStatus() const { return status; }

bool User::IsInGroup(const std::string& groupname) const{
    return groups.contains(groupname);
}

}