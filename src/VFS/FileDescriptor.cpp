#include "../../include/VFS/FileDescriptor.hpp"
#include "../../include/VFS/Utils.hpp"

namespace tmn_vfs {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// FilePermissions :

FilePermissions::FilePermissions(Permission user, Permission group, Permission other) :
    user(user), group(group), other(other) {}

std::string FilePermissions::PermissionToString(Permission p){
    if (p == Permission::READ) return "r-";
    if (p == Permission::WRITE) return "-w";
    if (p == Permission::READWRITE) return "rw";
    return "--";
}

std::string FilePermissions::toString() {
    std::string result = "";
    result += PermissionToString(user);
    result += PermissionToString(group);
    result += PermissionToString(other);
    return result;
}

FilePermissions::operator unsigned int() const{
    // '<<' is shift left
    return (static_cast<unsigned int>(user) << 6) |
            (static_cast<unsigned int>(group) << 3) |
            (static_cast<unsigned int>(other));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// FileDescriptor :

FileDescriptor::FileDescriptor(bool is_dir, const std::string& physical_path, const std::string& virtual_path, 
    const std::string& owner_user, const std::string& owner_group, FilePermissions file_permissions) :
    is_dir(is_dir), physical_path(physical_path), virtual_path(virtual_path), 
    owner_user(owner_user), owner_group(owner_group), 
    creation_time(GetTimeNow()), modification_time(GetTimeNow()), descriptor_modification_time(GetTimeNow()),
    file_permissions(file_permissions) {}

FileDescriptor::FileDescriptor(bool is_dir, const std::string& physical_path, const std::string& virtual_path, 
    const std::string& owner_user, FilePermissions file_permissions) :
    is_dir(is_dir), physical_path(physical_path), virtual_path(virtual_path), 
    owner_user(owner_user), owner_group(owner_user), 
    creation_time(GetTimeNow()), modification_time(GetTimeNow()), descriptor_modification_time(GetTimeNow()),
    file_permissions(file_permissions) {}

bool FileDescriptor::IsDirectory() const noexcept { return is_dir; }

const std::string& FileDescriptor::PhysicalPath() const noexcept { return physical_path; }

const std::string& FileDescriptor::VirtualPath() const noexcept { return virtual_path; }

const std::size_t FileDescriptor::Size() const noexcept { 
    if (is_dir){
        return inner_files.size();
    }
    return size; 
}

const std::string& FileDescriptor::OwnerUser() const noexcept { return owner_user; }

const std::string& FileDescriptor::OwnerGroup() const noexcept { return owner_group; }

const std::string& FileDescriptor::WhenCreated() const noexcept { return creation_time; }

const std::string& FileDescriptor::WhenModified() const noexcept { return modification_time; }

void FileDescriptor::AddInnerFile(const std::string& filename) {
    inner_files.insert(filename);
}

tmn_associative::HashSet<std::string> FileDescriptor::InnerFiles() {
    if (is_dir){
        return inner_files;
    }
    return tmn_associative::HashSet<std::string>();
}

nlohmann::json FileDescriptor::to_json() const noexcept {
    nlohmann::json fd_json;
    fd_json["is_dir"] = is_dir;
    fd_json["physical_path"] = physical_path;
    fd_json["virtual_path"] = virtual_path;
    fd_json["size"] = size;
    fd_json["owner_user"] = owner_user;
    fd_json["owner_group"] = owner_group;
    fd_json["creation_time"] = creation_time;
    fd_json["modification_time"] = modification_time;
    fd_json["descriptor_modification_time"] = descriptor_modification_time;

    nlohmann::json permissions_json;
    permissions_json["user"] = static_cast<unsigned int>(file_permissions.user);
    permissions_json["group"] = static_cast<unsigned int>(file_permissions.group);
    permissions_json["other"] = static_cast<unsigned int>(file_permissions.other);
    fd_json["permissions"] = permissions_json;

    nlohmann::json inner_files_json;
    for (const auto& inner_file : inner_files) {
        inner_files_json.push_back(inner_file);
    }
    fd_json["inner_files"] = inner_files_json;

    return fd_json;
}

}