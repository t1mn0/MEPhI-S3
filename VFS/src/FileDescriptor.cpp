#include <sstream>
#include <cstdint>

#include "../include/FileDescriptor.hpp"
#include "../include/Utils.hpp"

namespace tmn_vfs {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// FilePermissions :

FilePermissions::FilePermissions(Permission user, Permission group, Permission other) :
    user(user), group(group), other(other) {}

std::string FilePermissions::permission_to_string(Permission p){
    if (p == Permission::READ) return "r-";
    if (p == Permission::WRITE) return "-w";
    if (p == Permission::READWRITE) return "rw";
    return "--";
}

std::string FilePermissions::to_string() {
    std::string result = "";
    result += permission_to_string(user);
    result += permission_to_string(group);
    result += permission_to_string(other);
    return result;
}

FilePermissions::operator uint8_t() const{
    // '<<' is shift left
    return (static_cast<uint8_t>(user) << 6) |
            (static_cast<uint8_t>(group) << 3) |
            (static_cast<uint8_t>(other));
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// FileDescriptor :

FileDescriptor::FileDescriptor(
    uint64_t fd_id, bool is_dir, 
    uint64_t physical_file_id, uint64_t parent_dir_id, std::string filename,
    uint64_t content_offset, uint64_t content_size,
    uint64_t owner_user, uint64_t owner_group,
    const std::string& creation_time, const std::string& modification_time, const std::string& descriptor_modification_time,
    FilePermissions file_permissions)
    :
    fd_id(fd_id), is_dir(is_dir), physical_file_id(physical_file_id), parent_dir_id(parent_dir_id), filename(filename),
    content_offset(content_offset), content_size(content_size), owner_user(owner_user), owner_group(owner_group),
    creation_time(creation_time), modification_time(modification_time), descriptor_modification_time(descriptor_modification_time),
    file_permissions(file_permissions) 
    {}

FileDescriptor::FileDescriptor(
    uint64_t fd_id, bool is_dir, 
    uint64_t physical_file_id, uint64_t parent_dir_id, std::string filename,
    uint64_t content_offset, uint64_t content_size,
    uint64_t owner_user, uint64_t owner_group,
    FilePermissions file_permissions)
    :
    fd_id(fd_id), is_dir(is_dir), physical_file_id(physical_file_id), parent_dir_id(parent_dir_id), filename(filename),
    content_offset(content_offset), content_size(content_size), owner_user(owner_user), owner_group(owner_group),
    creation_time(get_time_now()), modification_time(get_time_now()), descriptor_modification_time(get_time_now()),
    file_permissions(file_permissions) 
    {}

FileDescriptor::FileDescriptor(
    uint64_t fd_id, bool is_dir, 
    uint64_t physical_file_id, uint64_t parent_dir_id, std::string filename,
    uint64_t content_offset, uint64_t content_size,
    uint64_t owner_user,
    FilePermissions file_permissions)
    :
    fd_id(fd_id), is_dir(is_dir), physical_file_id(physical_file_id), parent_dir_id(parent_dir_id), filename(filename),
    content_offset(content_offset), content_size(content_size), owner_user(owner_user), owner_group(owner_user),
    creation_time(get_time_now()), modification_time(get_time_now()), descriptor_modification_time(get_time_now()),
    file_permissions(file_permissions) 
    {}

FileDescriptor::FileDescriptor(const FileDescriptor& other) :
    fd_id(other.fd_id),
    is_dir(other.is_dir),
    physical_file_id(other.physical_file_id),
    parent_dir_id(other.parent_dir_id),
    filename(other.filename),
    content_offset(other.content_offset),
    content_size(other.content_size),
    owner_user(other.owner_user),
    owner_group(other.owner_group),
    creation_time(other.creation_time),
    modification_time(other.modification_time),
    descriptor_modification_time(other.descriptor_modification_time),
    file_permissions(other.file_permissions) {
    for (const auto& fdid : other.inner_files) {
        inner_files.insert(fdid);
    }
}

FileDescriptor& FileDescriptor::operator=(const FileDescriptor& other) noexcept {
    fd_id = other.fd_id;
    is_dir = other.is_dir;
    physical_file_id = other.physical_file_id;
    parent_dir_id = other.parent_dir_id;
    filename = other.filename;
    content_offset = other.content_offset;
    content_size = other.content_size;
    owner_user = other.owner_user;
    owner_group = other.owner_group;
    creation_time = other.creation_time;
    modification_time = other.modification_time;
    descriptor_modification_time = other.descriptor_modification_time;
    file_permissions = other.file_permissions;
    for (const auto& fdid : other.inner_files) {
        inner_files.insert(fdid);
    }

    return *this;
}

FileDescriptor::FileDescriptor(FileDescriptor&& other) :
    fd_id(other.fd_id),
    is_dir(other.is_dir),
    physical_file_id(other.physical_file_id),
    parent_dir_id(other.parent_dir_id),
    filename(std::move(other.filename)),
    content_offset(other.content_offset),
    content_size(other.content_size),
    owner_user(std::move(owner_user)),
    owner_group(std::move(owner_group)),
    creation_time(std::move(other.creation_time)),
    modification_time(std::move(other.modification_time)),
    descriptor_modification_time(std::move(other.descriptor_modification_time)),
    file_permissions(std::move(other.file_permissions)),
    inner_files(std::move(other.inner_files)) { 
        other.fd_id = 0;
        other.physical_file_id = 0;
        other.parent_dir_id = 0;
        other.filename = "";
        other.content_offset = 0;
        other.content_size = 0;
        other.owner_user = 0;
        other.owner_group = 0;
        other.creation_time = "";
        other.modification_time = "";
        other.descriptor_modification_time = "";
        other.inner_files = tmn_associative::HashSet<uint64_t>();
    }

FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other) noexcept {
    fd_id = other.fd_id;
    is_dir = other.is_dir;
    physical_file_id = other.physical_file_id;
    parent_dir_id = other.parent_dir_id;
    filename = std::move(other.filename);
    content_offset = other.content_offset;
    content_size = other.content_size;
    owner_user = std::move(owner_user);
    owner_group = std::move(owner_group);
    creation_time = std::move(other.creation_time);
    modification_time = std::move(other.modification_time);
    descriptor_modification_time = std::move(other.descriptor_modification_time);
    file_permissions = std::move(other.file_permissions);
    inner_files = std::move(other.inner_files);
    other.fd_id = 0;
    other.physical_file_id = 0;
    other.parent_dir_id = 0;
    other.content_offset = 0;
    other.content_size = 0;
    other.owner_user = 0;
    other.owner_group = 0;
    other.creation_time = "";
    other.modification_time = "";
    other.descriptor_modification_time = "";
    other.inner_files = tmn_associative::HashSet<uint64_t>();

    return *this;
}

FileDescriptor::~FileDescriptor() {
    inner_files.clear();
}

void FileDescriptor::modificate_content_now() noexcept {
    modification_time = get_time_now();
}

void FileDescriptor::modificate_descriptor_now() noexcept {
    descriptor_modification_time = get_time_now();
}

std::string FileDescriptor::to_string() const noexcept {
    std::string fd_string = "";
    fd_string.reserve(128);
    fd_string += "fd_id:"; fd_string += std::to_string(fd_id);
    fd_string += " is_dir:"; fd_string += (is_dir ? "1" : "0");
    fd_string += " ph_path:"; fd_string += std::to_string(physical_file_id);
    fd_string += " parent_dir:"; fd_string += std::to_string(parent_dir_id);
    fd_string += " filename:"; fd_string += filename;
    fd_string += " content_offset:"; fd_string += std::to_string(content_offset);
    fd_string += " content_size:"; fd_string += std::to_string(content_size);
    fd_string += " owner_u:"; fd_string += std::to_string(owner_user);
    fd_string += " owner_g:"; fd_string += std::to_string(owner_group);
    fd_string += " cr_time:"; fd_string += creation_time;
    fd_string += " mod_time:"; fd_string += modification_time;
    fd_string += " d_mod_time:"; fd_string += descriptor_modification_time;

    fd_string += " <";
    fd_string += std::to_string(static_cast<uint8_t>(file_permissions.user));
    fd_string += std::to_string(static_cast<uint8_t>(file_permissions.group));
    fd_string += std::to_string(static_cast<uint8_t>(file_permissions.other));
    fd_string += ">";

    fd_string += " [";
    for (const auto& innerfile_id : inner_files) {
        fd_string += std::to_string(innerfile_id);
        fd_string += ",";
    }
    fd_string += "]";

    return fd_string;
}

FileDescriptor FileDescriptor::from_string(const std::string& fd_string) {
    FileDescriptor fd;
    std::stringstream ss(fd_string);
    std::string segment = "-";

    // fd_id
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.fd_id = std::stoul(segment);

    // is_dir
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.is_dir = (segment == "1");

    // ph_path
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.physical_file_id = std::stoul(segment);

    // parent_dir
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.parent_dir_id = std::stoul(segment);

    // filename
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.filename = segment;

    // content_offset
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.content_offset = std::stoul(segment);

    // content_size
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.content_size = std::stoul(segment);

    // owner_u
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.owner_user = std::stoul(segment);

    // owner_g
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.owner_group = std::stoul(segment);

    // cr_time
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.creation_time = segment;

    // mod_time
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.modification_time = segment;

    // d_mod_time
    std::getline(ss, segment, ':');
    std::getline(ss, segment, ' ');
    fd.descriptor_modification_time = segment;

    // permissions
    std::getline(ss, segment, '<');
    std::getline(ss, segment, '>');

    fd.file_permissions.user = static_cast<Permission>(std::stoul(segment.substr(0,1)));
    fd.file_permissions.group = static_cast<Permission>(std::stoul(segment.substr(1,1)));
    fd.file_permissions.other = static_cast<Permission>(std::stoul(segment.substr(2,1)));

    std::getline(ss, segment, '[');
    std::getline(ss, segment, ']');

    std::stringstream inner_ss(segment);
    std::string inner_id_str;
    while (std::getline(inner_ss, inner_id_str, ',')) {
       if(inner_id_str.length() > 0){
          std::size_t pos = inner_id_str.find_last_not_of(" \t\n\r");
          fd.inner_files.insert(std::stoul(inner_id_str.substr(0, pos + 1)));
       }
    }

    return fd;
}

}