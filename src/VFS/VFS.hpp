#pragma once

#include <string>
#include <unordered_map>

#include "../../include/Associative/HashTable.hpp"

#include "../../include/VFS/User.hpp"
#include "../../include/VFS/Group.hpp"
#include "../../include/VFS/FileInfo.hpp"

namespace tmn_vfs {

class VirtualFileSystem {
private:
    VirtualFileSystem() = default;
    static void MountVFS();

private:
    // Fields :
    std::string active_user;
    std::string current_directory;
    uint32_t active_super; 
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
    const std::string& WhoUser() const; // * DONE
    // bool IsGoodPath() const;

    // Initializing :
    [[nodiscard]]
    static const VirtualFileSystem& Init(const User& user); // * DONE

    // Users policy & actions :
    const std::string AddUser(const User& user); // * DONE
    void Authorization(const std::string& username, const std::string& password_hash); // * DONE
    bool IsUserInSystem(const std::string& username);
    void RemoveUser(const std::string& username);

    // Groups policy & actions :
    const std::string AddGroup(const Group& group);
    void AddToGroup(const std::string& username, const std::string& groupname, const std::string& password_to_group);
    bool IsGroupInSystem(const std::string& groupname);
    void RemoveGroup(const std::string& groupname);

    // Actions with files :
    void CreateFile(const FileInfo& file_info); // TODO
    void MoveFile(const std::string& path_from, const std::string& path_to); 
    // void CopyFileContent(const std::string& path_from, const std::string& path_to);
    void RemoveFile(const std::string& filename, bool is_recursive);
    const FileInfo& GetFileInfo(const std::string& filename); // TODO

    // Relocation user :
    void GoTo(const std::string& path);

    // Remove system :
    static void Shutdown();
};
    
};
