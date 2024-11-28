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
    static bool VFSInSystem();
    static bool IsValidSystemFile(const std::filesystem::path& path);
    unsigned long FindRecordingFiles(const std::filesystem::path& path, unsigned long parent_id) noexcept; 
    void InitUsers();
    void InitGroups();
    void InitFileDescriptors();
    void InitSystemParameters();

private:
// Support functions for shutdown :
    void SaveConfig() const noexcept;
    void SaveDescriptors() const noexcept;
    void SaveGroups() const noexcept;
    void SaveUsers() const noexcept;
    void SaveAll() const noexcept;

private:
    // Fields :
    unsigned long recording_file = 0;
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
    std::string PWD() const;
    std::size_t CountUser() const;
    std::size_t CountGroup() const;
    std::size_t CountMembers(const std::string& groupname) const;
    bool IsUserInSystem(const std::string& username) const;
    bool IsGroupInSystem(const std::string& groupname) const;
    bool IsFileInCurrentDir(const std::string& filename) const;
    bool IsMember(const std::string& groupname, const std::string& username) const;

    // Initializing :
    [[nodiscard]]
    static VirtualFileSystem Init();

    // Users policy & actions :
    bool AddUser(User& user);
    bool Authorization(const std::string& username, const std::string& password_hash);
    bool RemoveUser(const std::string& username);

    // Groups policy & actions :
    bool AddGroup(Group& group);
    bool AddUserToGroup(const std::string& username, const std::string& groupname);
    bool RemoveUserFromGroup(const std::string& username, const std::string& groupname);
    bool RemoveGroup(const std::string& groupname);

    // Actions with files :
    tmn_associative::HashSet<std::string> CurrentDirContent();
    bool AddFile(const FileDescriptor& fd, const std::string& content = "");
    bool RenameFile(const std::string& old_filename, const std::string& new_filename);
    //void NewContent(const std::string& filename, const std::string& content);
    bool RemoveFile(const std::string& filename);
    bool RemoveDir(const std::string& filename, bool is_recursive);

    tmn_sequence::ArraySequence<unsigned long> FindFileByName(const std::string& filename, bool in_current_dir = true);
    tmn::Optional<std::string> DoPath(unsigned long) const noexcept;
    unsigned long NextRecordFile() noexcept;

    void Tree(bool only_one_level = false) const;

    // Relocation user :
    bool GoTo(std::string& path);

    friend class View;
};
    
}