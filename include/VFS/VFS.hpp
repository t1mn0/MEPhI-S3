#pragma once

#include <string>

#include "../Associative/HashTable.hpp"
#include "../Sequence/ArraySequence.hpp"

#include "User.hpp"
#include "Group.hpp"
#include "FileDescriptor.hpp"

namespace tmn_vfs {

inline const std::string ENTRY_POINT("../../examples/FS");
inline const std::string GROUPS("../../examples/groups.json");
inline const std::string USERS("../../examples/users.json");
inline const std::string FILE_DESCRIPTORS("../../examples/fd.json");
inline const std::string VFS_INFO("../../examples/vfs.dat");

inline tmn_associative::HashSet<std::string> AllowedTextExtensions = {".txt", ".md", ".doc"};

class VirtualFileSystem {
public:
// Support structures : 
    struct Path{
    private:
        std::string path = "FS";
    public:
        Path() = default;
        Path(const std::string& path);
        std::string ParentFileName() const noexcept;
        std::string FileName() const noexcept;
        std::string FileExtension() const noexcept;
        std::string toString() const;

        bool operator==(const VirtualFileSystem::Path& other) const noexcept;
    };

private:
// Support functions for initialization :
    VirtualFileSystem() = default;
    static tmn_sequence::ArraySequence<std::string> FindRecordingFiles() noexcept; 
    static bool VFSInSystem();
    static void InstallHelperFiles();
    void InitSystemParameters();
    void InitFileDescriptors();
    void InitGroups();
    void InitUsers();

private:
// Support functions for shutdown :
    void SaveSystemParameters() const noexcept;
    void SaveDescriptors() const noexcept;
    void SaveGroups() const noexcept;
    void SaveUsers() const noexcept;
    void SaveAll() const noexcept;
    tmn_associative::HashSet<std::string> writable_files; // as paths to physical files

private:
    // Fields :
    std::string active_user;
    uint8_t active_super = 0; // user-authorization gives 5 applications of the active user's capabilities
    std::string current_directory = "FS";
    tmn_associative::HashTable<Path, FileDescriptor> index_table;
    tmn_associative::HashTable<std::string, tmn_associative::HashSet<std::string>> possible_paths; // ? ? ?
    tmn_associative::HashTable<std::string, User> users_table;
    tmn_associative::HashTable<std::string, Group> groups_table;

public:
    ~VirtualFileSystem();
    
    // Checkers :
    const std::string& WhoUser() const;
    const std::string& PWD() const; // Present Working Dir
    std::size_t CountUser() const;
    std::size_t CountGroup() const;
    std::size_t CountMembers(const std::string& groupname) const;
    bool IsUserInSystem(const std::string& username) const;
    bool IsGroupInSystem(const std::string& groupname) const;
    bool IsPathInSystem(const Path& path) const;
    bool IsMember(const std::string& groupname, const std::string& username) const;

    // Initializing :
    [[nodiscard]]
    static VirtualFileSystem Init();

    // Users policy & actions :
    void AddUser(const User& user);
    tmn::Optional<User> GetUserInfo(const std::string& username) const;
    tmn_sequence::ArraySequence<std::string> AllUsers() const;
    tmn_associative::HashSet<std::string> AllUserGroups(const std::string& username) const;
    void Authorization(const std::string& username, const std::string& password_hash);
    void RemoveUser(const std::string& username);

    // Groups policy & actions :
    void AddGroup(const Group& group);
    void AddToGroup(const std::string& username, const std::string& groupname);
    tmn::Optional<Group> GetGroupInfo(const std::string& groupname) const;
    tmn_sequence::ArraySequence<std::string> GetAllGroups() const;
    void RemoveFromGroup(const std::string& username, const std::string& groupname);
    void RemoveGroup(const std::string& groupname);

    // Actions with files :
    tmn_associative::HashSet<std::string> CurrentDirContent();
    void AddFile(const FileDescriptor& file_info);
    // TODO : void MoveFile(const std::string& path_from, const std::string& path_to);
    // TODO : void RemoveFile(const std::string& filename, bool is_recursive);
    const FileDescriptor& GetFileInfo(const std::string& filename); 

    // Relocation user :
    void GoTo(const std::string& path);
};
    
}