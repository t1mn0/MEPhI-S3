#pragma once

#include <string>
#include <filesystem>

#include "../../include/Associative/HashTable.hpp"
#include "../../include/Sequence/ArraySequence.hpp"

#include "User.hpp"
#include "Group.hpp"
#include "FileDescriptor.hpp"
#include "vfs_constants.hpp"

namespace tmn_vfs {

class View;

class VirtualFileSystem {
private:
// Support functions for initialization :
    static void CreateHelperFiles();
    static bool IsValidSystemFile(const std::filesystem::path& path);

    void InitUsers();
    void InitGroups();
    void InitFileDescriptors();
    void InitSystemParameters();

private:
// Support functions for shutdown :
    void SaveConfig() const;
    void SaveDescriptors() const;
    void SaveGroups() const;
    void SaveUsers() const;
    void SaveAll() const;

private:
// Fields :
    unsigned long rec_id = 0;
    unsigned long fd_id = 0;
    unsigned long user_id = 1;
    unsigned long group_id = 1;

    unsigned long active_user;
    unsigned long current_directory = 0;

    tmn_associative::HashTable<std::string, unsigned long> usernames;
    tmn_associative::HashTable<std::string, unsigned long> groupnames;
    tmn_associative::HashTable<unsigned long, std::string> recording_files;
    tmn_associative::HashTable<unsigned long, FileDescriptor> files;
    tmn_associative::HashTable<unsigned long, User> users_table;
    tmn_associative::HashTable<unsigned long, Group> groups_table;

public:
    ~VirtualFileSystem();
    
// Checkers :
    std::string PWD() const noexcept;
    std::size_t CountUser() const noexcept;
    std::size_t CountGroup() const noexcept;
    tmn::Optional<std::size_t> CountMembers(const std::string& groupname) const noexcept;

// Initializing :
    [[nodiscard]]
    static VirtualFileSystem Init(std::string root_password = "");

// Users policy & actions :
    void AddUser(User& user);
    void Authorization(const std::string& username, const std::string& password_hash);
    void RemoveUser(const std::string& username);

// Groups policy & actions :
    void AddGroup(Group& group);
    void AddUserToGroup(const std::string& username, const std::string& groupname);
    void RemoveUserFromGroup(const std::string& username, const std::string& groupname);
    void RemoveGroup(const std::string& groupname);

// Actions with files :
    tmn_associative::HashSet<std::string> CurrentDirContent() const noexcept;
    void AddFileContent(const std::string& filename, const std::string& content);
    std::string GetFileContent(const std::string& filename);
    void AddFile(FileDescriptor);
    void ChangeFilePermissions(const std::string& filename, unsigned int perm);
    void SetOwnerGroup(unsigned long fd_id, unsigned long group_id);
    void RenameFile(const std::string& old_filename, const std::string& new_filename);
    void RemoveFileContent(const std::string& filename);
    void RemoveFile(const std::string& filename);
    void RemoveDir(const std::string& filename, bool is_recursive);

// Relocation user & other methods :
    static bool VFSInSystem();
    
    void GoTo(std::string& path);
    tmn::Optional<std::string> DoPath(unsigned long) const noexcept;
    tmn_sequence::ArraySequence<unsigned long> FindFileByName(const std::string& filename, bool in_current_dir = true) const noexcept ;
    bool HavePermission(unsigned long fd_id, unsigned long user_id, unsigned int perm) const;
    

    friend class View;
};
    
}