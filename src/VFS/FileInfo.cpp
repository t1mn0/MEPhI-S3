#include "../../include/VFS/FileInfo.hpp"
#include "../../include/VFS/Utils.hpp"

namespace tmn_vfs {

// Permissions :
Permissions operator|(Permissions a, Permissions b) {
  return static_cast<Permissions>(static_cast<unsigned char>(a) | static_cast<unsigned char>(b));
}

// Constructors :
FileInfo::FileInfo(bool is_dir, const std::string& physical_path, const std::string& virtual_path, const std::string& creator_name) :
    is_dir(is_dir), physical_path(physical_path), virtual_path(virtual_path), 
    creator_name(creator_name), creation_time(GetTimeNow()), modification_time(GetTimeNow()) { }

// Getters & setters :
bool FileInfo::IsDirectory() const { return is_dir; }
const std::string& FileInfo::GetVirtualPath() const { return virtual_path; }
const std::size_t FileInfo::GetSize() const { return inner_files.size(); }
const std::string& FileInfo::GetCreatorName() const { return creator_name; }
const std::string& FileInfo::GetCreationTime() const { return creation_time; }
const std::string& FileInfo::GetModificationTime() const { return modification_time; }
void FileInfo::SetModificationTime(const std::string& time) { modification_time = time; }

// Modifiers :
void FileInfo::AddInnerFile(const std::string& inner_filename) {
    inner_files.insert(inner_filename);
}

void FileInfo::AddGroup(const std::string& groupname, Permissions permissions) {
    permissions_table[groupname] = permissions;
}

bool FileInfo::CanAccess(const std::string& groupname, Permissions permission) const {
    auto optional_user = permissions_table.get(groupname);
    if (optional_user.has_value()) {
        unsigned char user_permissions = static_cast<unsigned char>(optional_user.value());
        unsigned char requested_permissions = static_cast<unsigned char>(permission);
        return (user_permissions & requested_permissions) == requested_permissions;
    } 
    
    std::cerr << "Group with groupname = " << groupname << " is not found" << std::endl;
    return false;
}


}