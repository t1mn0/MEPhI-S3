#pragma once

#include <string>
#include <nlohmann/json.hpp>

#include "../Associative/HashSet.hpp"
#include "../Associative/HashTable.hpp"

namespace tmn_vfs {

class VirtualFileSystem;

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

class FileDescriptor {
private:
  bool is_dir;
  std::string physical_path; // where the content is
  std::string virtual_path; // what the user sees
  unsigned int size = 0; // for regular file: in bytes; for dirs: inner_files.size();
  std::string owner_user; 
  std::string owner_group;
  std::string creation_time;
  std::string modification_time; // for content
  std::string descriptor_modification_time; // for meta
  FilePermissions file_permissions;

  tmn_associative::HashSet<std::string> inner_files;


public:
  FileDescriptor() = default; 

  FileDescriptor(bool is_dir, const std::string& physical_path, const std::string& virtual_path, 
  const std::string& owner_user, const std::string& owner_group,
  FilePermissions file_permissions = FilePermissions(Permission::READWRITE, Permission::READWRITE, Permission::NONE));

  FileDescriptor(bool is_dir, const std::string& physical_path, const std::string& virtual_path, 
  const std::string& owner_user,
  FilePermissions file_permissions = FilePermissions(Permission::READWRITE, Permission::READWRITE, Permission::NONE));

  // Getters :
  bool IsDirectory() const noexcept;
  const std::string& PhysicalPath() const noexcept;
  const std::string& VirtualPath() const noexcept;
  const std::size_t Size() const noexcept;
  const std::string& OwnerUser() const noexcept;
  const std::string& OwnerGroup() const noexcept;
  const std::string& WhenCreated() const noexcept;
  const std::string& WhenModified() const noexcept; // for modification_time (content, not meta-)
  tmn_associative::HashSet<std::string> InnerFiles();

  // Modifiers :
  void AddInnerFile(const std::string& filename);
  nlohmann::json to_json() const noexcept;

  // Other :
  friend class VirtualFileSystem;
};

}