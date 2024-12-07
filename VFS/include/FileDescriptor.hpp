#pragma once

#include <string>
#include <cstdint>

#include "../../include/Associative/HashSet.hpp"
#include "vfs_constants.hpp"

namespace tmn_vfs {

class VirtualFileSystem;
class View;

enum class Permission : uint8_t {
  NONE = 0,       // 000        string: --
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

  explicit operator uint8_t() const;

  std::string to_string();

private:
  std::string permission_to_string(Permission p);
};

struct FileDescriptor {
private:
  uint64_t fd_id;
  bool is_dir;
  uint64_t physical_file_id = 0;                      
  uint64_t parent_dir_id = 0;                       
  std::string filename = "file";                          
  uint64_t content_offset = 0;                          
  uint64_t content_size = 0;                           
  uint64_t owner_user = 0; 
  uint64_t owner_group = 0;
  std::string creation_time = "-";
  std::string modification_time = "-";                       
  std::string descriptor_modification_time = "-";      
  FilePermissions file_permissions;

  tmn_associative::HashSet<uint64_t> inner_files;

public:
  FileDescriptor() : file_permissions(Permission::READWRITE, Permission::READWRITE, Permission::READ) {}
  FileDescriptor(const FileDescriptor&);
  FileDescriptor(FileDescriptor&&);
  FileDescriptor& operator=(const FileDescriptor&) noexcept;
  FileDescriptor& operator=(FileDescriptor&&) noexcept;
  ~FileDescriptor();

  FileDescriptor(
    uint64_t fd_id, bool is_dir, 
    uint64_t physical_file_id, uint64_t parent_dir_id, std::string filename, 
    uint64_t content_offset, uint64_t content_size,
    uint64_t owner_user, uint64_t owner_group,
    const std::string& creation_time, const std::string& modification_time, const std::string& descriptor_modification_time,
    FilePermissions file_permissions = FilePermissions(Permission::READWRITE, Permission::READ, Permission::NONE));

  FileDescriptor(
    uint64_t fd_id, bool is_dir, 
    uint64_t physical_file_id, uint64_t parent_dir_id, std::string filename, 
    uint64_t content_offset, uint64_t content_size,
    uint64_t owner_user, uint64_t owner_group,
    FilePermissions file_permissions = FilePermissions(Permission::READWRITE, Permission::READ, Permission::NONE));

  FileDescriptor(
    uint64_t fd_id, bool is_dir, 
    uint64_t physical_file_id, uint64_t parent_dir_id, std::string filename,
    uint64_t content_offset, uint64_t content_size,
    uint64_t owner_user,
    FilePermissions file_permissions = FilePermissions(Permission::READWRITE, Permission::READ, Permission::NONE));

  void modificate_content_now() noexcept;
  void modificate_descriptor_now() noexcept;

  std::string to_string() const noexcept;
  static FileDescriptor from_string(const std::string& fd_string);

  friend class VirtualFileSystem;
  friend class View;
};

}