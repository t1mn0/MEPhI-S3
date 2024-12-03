#pragma once

#include <string>

#include "../include/VFS.hpp"

namespace tmn_vfs {

class View {
private:
    VirtualFileSystem vfs;
public:
// Run parser :
    void run();

    View();

    View(std::string password);

// Help :
    void global_help() const noexcept;

    void users_help() const noexcept;
    void whoami_help() const noexcept;
    void usercount_help() const noexcept;
    void login_help() const noexcept;
    void newuser_help() const noexcept;
    
    void groups_help() const noexcept;
    void mygroups_help() const noexcept;
    void groupcount_help() const noexcept;
    void newgroup_help() const noexcept;
    void addtogroup_help() const noexcept;
    void rmmefromgroup_help() const noexcept;
    void rmgroup_help() const noexcept;

    void mkdir_help() const noexcept;
    void mkfile_help() const noexcept;
    void addcontent_help() const noexcept;
    void setgroup_help() const noexcept;
    void renamefile_help() const noexcept;
    void rmdir_help() const noexcept;
    void rmfile_help() const noexcept;
    void cat_help() const noexcept;
    void ls_help() const noexcept;
    void cd_help() const noexcept;
    void find_help() const noexcept;

    void pwd_help() const noexcept;
    void clear_help() const noexcept;
    void help_help() const noexcept;
    void exit_help() const noexcept;

// Commands :
    void users(bool v = false) const noexcept;
    void whoami(bool v = false) const noexcept;
    void usercount() const noexcept;
    void login(const std::string& username, const std::string& password_hash) noexcept;
    void newuser(const std::string& username, const std::string& fullname, const std::string& password_hash, bool st = false) noexcept;
    
    void groups(bool v = false) const noexcept;
    void mygroups(bool v = false) const noexcept;
    void groupcount() const noexcept;
    void newgroup(const std::string& groupname) noexcept;
    void addtogroup(const std::string& username, const std::string& groupname) noexcept;
    void rmmefromgroup(const std::string& groupname) noexcept;
    void rmgroup(const std::string& groupname) noexcept;

    void mkdir(const std::string& dirname, std::string path = "") noexcept;
    void mkfile(const std::string& filename, std::filesystem::path, std::string path = "") noexcept;
    // ! void addcontent(const std::string& path, std::string content) noexcept;
    void setgroup(const std::string &filename, const std::string& groupname) noexcept;
    void renamefile(const std::string& old_filename, const std::string& new_filename) noexcept;
    // ! void rmdir(const std::string& dirname, std::string&& path = "", bool r = false) noexcept;
    // ! void rmfile(const std::string& dirname, std::string&& path = "") noexcept;
    void cat(const std::string& filename, std::string path = "") noexcept;
    void ls(bool v = false) noexcept;
    void cd(std::string& path) noexcept;
    void find(short filetype, bool where, const std::string& name_pattern) noexcept;

    void pwd() const noexcept;
    void clear() const noexcept;
};

}