#pragma once

#include <string>

#include "../../include/Associative/HashSet.hpp"
#include "vfs_constants.hpp"

namespace tmn_vfs {

class VirtualFileSystem;
class View;

enum class Permission : unsigned int {
  NONE = 0,       //            string: --
  READ = 1,       // 001        string: r-
  WRITE = 2,      // 010        string: -w
  READWRITE = 3   // 011        string: rw
};

struct FilePermissions {
  Permission user; // owner-user
  Permission group; // owner-group
  Permission other;

  FilePermissions() = default;
  FilePermissions(Permission user, Permission group, Permission other);

  explicit operator unsigned int() const;

  std::string toString();

private:
  std::string PermissionToString(Permission p);
};

struct FileDescriptor {
private:
  unsigned long fd_id;
  bool is_dir;
  unsigned long physical_file_id = 0;                      
  unsigned long parent_dir_id = 0;                       
  std::string filename = "file";                          
  unsigned long content_offset = 0;                          
  unsigned long content_size = 0;                           
  unsigned long owner_user = 0; 
  unsigned long owner_group = 0;
  std::string creation_time = "-";
  std::string modification_time = "-";                       
  std::string descriptor_modification_time = "-";      
  FilePermissions file_permissions;

  tmn_associative::HashSet<unsigned long> inner_files;

public:
  FileDescriptor() : file_permissions(Permission::READWRITE, Permission::READWRITE, Permission::READ) {}
  FileDescriptor(const FileDescriptor&);
  FileDescriptor(FileDescriptor&&);
  FileDescriptor& operator=(const FileDescriptor&) noexcept;
  FileDescriptor& operator=(FileDescriptor&&) noexcept;
  ~FileDescriptor();

  FileDescriptor(
    unsigned long fd_id, bool is_dir, 
    unsigned long physical_file_id, unsigned long parent_dir_id, std::string filename, 
    unsigned long content_offset, unsigned long content_size,
    unsigned long owner_user, unsigned long owner_group,
    const std::string& creation_time, const std::string& modification_time, const std::string& descriptor_modification_time,
    FilePermissions file_permissions = FilePermissions(Permission::READWRITE, Permission::READ, Permission::NONE));

  FileDescriptor(
    unsigned long fd_id, bool is_dir, 
    unsigned long physical_file_id, unsigned long parent_dir_id, std::string filename, 
    unsigned long content_offset, unsigned long content_size,
    unsigned long owner_user, unsigned long owner_group,
    FilePermissions file_permissions = FilePermissions(Permission::READWRITE, Permission::READ, Permission::NONE));

  FileDescriptor(
    unsigned long fd_id, bool is_dir, 
    unsigned long physical_file_id, unsigned long parent_dir_id, std::string filename,
    unsigned long content_offset, unsigned long content_size,
    unsigned long owner_user,
    FilePermissions file_permissions = FilePermissions(Permission::READWRITE, Permission::READ, Permission::NONE));

  std::string toString() const noexcept;
  static FileDescriptor fromString(const std::string& fd_string);

  friend class VirtualFileSystem;
  friend class View;
};

}