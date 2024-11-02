#pragma once

#include <string>

#include "../../include/Associative/HashTable.hpp"

#include "../../include/VFS/User.hpp"
#include "../../include/VFS/Group.hpp"
#include "../../include/VFS/FileInfo.hpp"

namespace tmn_vfs {

uint16_t FILE_COUNT = 12;
std::string MOUNT_DIR = "../../examples";
std::string VFS_DIR = "VFS";

class VirtualFileSystem {
private:
    VirtualFileSystem() = default;
    static void MountVFS();

public:
    // Fields :
    std::string active_user;
    std::string current_directory = "files";
    uint32_t active_super = 0; 
    tmn_associative::HashTable<std::string, FileInfo> index_table;
    tmn_associative::HashTable<std::string, User> users_table;
    tmn_associative::HashTable<std::string, Group> groups_table;

public:
    //VirtualFileSystem(const VirtualFileSystem&) = delete; 
    //VirtualFileSystem(VirtualFileSystem&&) = delete; 
    //void operator=(const VirtualFileSystem&) = delete;
    //void operator=(VirtualFileSystem&&) = delete;
    ~VirtualFileSystem();
    
    // Checkers :
    const std::string& WhoUser() const;
    const std::string& PWD() const;
    std::size_t CountUser() const;
    std::size_t CountGroup() const;
    std::size_t CountMembers(const std::string& groupname) const;
    bool IsUserInSystem(const std::string& username) const;
    bool IsGroupInSystem(const std::string& groupname) const;
    bool IsMember(const std::string& groupname, const std::string& username) const;
    bool IsGoodPath(const std::string& path) const;

    // Initializing :
    [[nodiscard]]
    static VirtualFileSystem Init(const User &user);

    // Users policy & actions :
    void AddUser(const User& user);
    void Authorization(const std::string& username, const std::string& password_hash); // * DONE
    void RemoveUser(const std::string& username);

    // Groups policy & actions :
    void AddGroup(const Group& group);
    void AddToGroup(const std::string& username, const std::string& groupname, const std::string& password_hash);
    void RemoveFromGroup(const std::string& username, const std::string& groupname, const std::string& password_hash);
    void RemoveGroup(const std::string& groupname);

    // Actions with files :
    void AddFile(const FileInfo& file_info);
    // TODO : void MoveFile(const std::string& path_from, const std::string& path_to);
    // TODO : void RemoveFile(const std::string& filename, bool is_recursive);
    const FileInfo& GetFileInfo(const std::string& filename); 

    // Relocation user :
    void GoTo(const std::string& path);

    // Remove system :
    static void Shutdown();
};
    
};
