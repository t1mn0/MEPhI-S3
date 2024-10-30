#pragma once

#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <string>
#include <iostream>

#include "../Associative/HashSet.hpp"
#include "../Associative/HashTable.hpp"

namespace tmn_vfs {

class VirtualFileSystem;

enum class Permissions : unsigned char {
  READ = 1,
  WRITE = 2,
  MOVE = 4,
  DELETE = 8
};

Permissions operator|(Permissions a, Permissions b);

class FileInfo {
private:
  bool is_dir;
  std::string physical_path; 
  std::string virtual_path;
  std::string creator_name;
  std::string creation_time;
  std::string modification_time;

  tmn_associative::HashSet<std::string> inner_files;
  tmn_associative::HashTable<std::string, Permissions> permissions_table; // GroupName: Permissions

public:
  FileInfo() = default; 
  FileInfo(bool is_dir, const std::string& physical_path, const std::string& virtual_path, const std::string& creator_name);

  // Getters & setters :
  bool IsDirectory() const;
  const std::string& GetVirtualPath() const;
  const std::size_t GetSize() const;
  const std::string& GetCreatorName() const;
  const std::string& GetCreationTime() const;
  const std::string& GetModificationTime() const;
  void SetModificationTime(const std::string& time);

  // Modifiers :
  void AddInnerFile(const std::string& inner_filename);
  void AddGroup(const std::string& groupname, Permissions permissions);

  // Checkers :
  bool CanAccess(const std::string& groupname, Permissions permission) const;

  friend class VirtualFileSystem;
};

};