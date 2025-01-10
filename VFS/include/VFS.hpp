#pragma once

#include <string>
#include <filesystem>
#include <cstdint>

#include "../../include/Associative/HashTable.hpp"
#include "../../include/Sequence/ArraySequence.hpp"

#include "User.hpp"
#include "Group.hpp"
#include "FileDescriptor.hpp"
#include "vfs_constants.hpp"

namespace tmn {
namespace vfs {

class View;

class VirtualFileSystem {
private:
// Support functions for initialization :
    static void create_helper_files();
    static bool is_valid_system_files(const std::filesystem::path& path);

    void init_users();
    void init_groups();
    void init_file_descriptors();
    void init_system_parameters();

private:
// Support functions for shutdown :
    void save_config() const;
    void save_file_descriptors() const;
    void save_groups() const;
    void save_users() const;
    void save_all() const;

private:
// Fields :
    uint64_t rec_id = 0;
    uint64_t fd_id = 0;
    uint64_t user_id = 1;
    uint64_t group_id = 1;

    uint64_t active_user;
    uint64_t current_directory = 0;

    tmn::associative::HashTable<std::string, uint64_t> usernames;
    tmn::associative::HashTable<std::string, uint64_t> groupnames;
    tmn::associative::HashTable<uint64_t, std::string> recording_files;
    tmn::associative::HashTable<uint64_t, FileDescriptor> files;
    tmn::associative::HashTable<uint64_t, User> users_table;
    tmn::associative::HashTable<uint64_t, Group> groups_table;

public:
    ~VirtualFileSystem();
    
// Checkers :
    std::string pwd() const noexcept;
    std::size_t count_users() const noexcept;
    std::size_t count_groups() const noexcept;
    tmn::Optional<std::size_t> count_members(const std::string& groupname) const noexcept;

// Initializing :
    [[nodiscard]]
    static VirtualFileSystem init(std::string root_password = "");

// Users policy & actions :
    void add_user(User& user);
    void login(const std::string& username, const std::string& password_hash);
    void remove_user(const std::string& username);

// Groups policy & actions :
    void add_group(Group& group);
    void add_user_to_group(const std::string& username, const std::string& groupname);
    void remove_user_from_group(const std::string& username, const std::string& groupname);
    void remove_group(const std::string& groupname);

// Actions with files :
    tmn::associative::HashSet<std::string> current_dir_content() const noexcept;
    void add_file_content(const std::string& filename, const std::string& content);
    std::string get_file_content(const std::string& filename);
    void add_file(FileDescriptor);
    void change_file_permissions(const std::string& filename, uint64_t perm);
    void set_owner_group(uint64_t fd_id, uint64_t group_id);
    void rename_file(const std::string& old_filename, const std::string& new_filename);
    void remove_file_content(const std::string& filename);
    void remove_file(const std::string& filename);
    void remove_dir(const std::string& filename, bool is_recursive);

// Relocation user & other methods :
    static bool in_system();
    
    void go_to(std::string& path);
    tmn::Optional<std::string> do_path(uint64_t) const noexcept;
    tmn::sequence::ArraySequence<uint64_t> find_file_by_name(const std::string& filename, bool in_current_dir = true) const noexcept ;
    bool have_permission(uint64_t fd_id, uint64_t user_id, uint8_t perm) const;
    

    friend class View;
};
    
}
}