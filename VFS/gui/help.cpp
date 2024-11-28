#include <iostream>
#include <string>

#include "View.hpp"

namespace tmn_vfs {

void View::global_help() const noexcept{
    std::cout << "These are common VFS commands used in various situations:" << std::endl;
	std::cout << "Handling users" << std::endl;
	std::cout << "  - users  : Show all users in the system" << std::endl;
	std::cout << "  - whoami  : Show the active user" << std::endl;
	std::cout << "  - usercount : Show the number of users on the vfs (without superuser)" << std::endl;
	std::cout << "  - login : User login to the vfs" << std::endl;
	std::cout << "  - newuser : Create new user" << std::endl;
	std::cout << "Handling groups" << std::endl;
	std::cout << "  - groups  : Show all groups in the system" << std::endl;
	std::cout << "  - mygroups : Show all user-groups in the system" << std::endl;
	std::cout << "  - groupcount : Show the number of groups on the vfs (without superuser's group)" << std::endl;
	std::cout << "  - newgroup : Create new group and add active user in it" << std::endl;
	std::cout << "  - addtogroup : Add the specified user to the specified group" << std::endl;
	std::cout << "  - rmmefromgroup : Remove the active user from the specified group" << std::endl;
	std::cout << "  - rmgroup : Remove the specified user group, if possible" << std::endl;
	std::cout << "Handling files" << std::endl;
	std::cout << "  - mkdir  : Creates a virtual directory along the specified path, if specified. Otherwise, in the current directory" << std::endl;
	std::cout << "  - mkfile  : Creates a virtual regular file along the specified path, if specified. Otherwise, in the current directory" << std::endl;
	std::cout << "  - rmdir  : Removes a virtual directory along the specified path, if specified. Otherwise, searches in the current directory" << std::endl;
	std::cout << "  - rmfile  : Removes a virtual regular file along the specified path, if specified. Otherwise, searches in the current directory" << std::endl;
	std::cout << "  - ls  : Show internal directory files internal directory files" << std::endl;
	std::cout << "  - cd  : Move between directories" << std::endl;
	std::cout << "  - find  : Search for a file or directory" << std::endl;
	std::cout << "Other" << std::endl;
	std::cout << "  - pwd  : Show the name of the current working directory" << std::endl;
	std::cout << "  - tree  : Show tree view of vfs" << std::endl;
	std::cout << "  - clear  : Clear the screen " << std::endl;
	std::cout << "  - help  : Show information on built-in commands" << std::endl;
	std::cout << "  - exit  : Show information on built-in commands" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Use [help] [command] for a detailed description of the commands and their modifications " << std::endl;
}

void View::users_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - users [options]" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Show all users in the system" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Options:" << std::endl << 
	"-v (verbose) " << "Detailed output: outputs user_id, username, fullname, creation time, status" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::whoami_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - whoami [options]" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Show the active user" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Options:" << std::endl << "-v (verbose) " << "Detailed output: outputs user_id, username, fullname, creation time, status" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::usercount_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - usercount" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Show the number of users on the vfs (without superuser)" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::login_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << "  login <username> <password>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "User login to the vfs" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::newuser_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - newuser [options] <username> <fullname> <password>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Create new user" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Options:" << std::endl << "-st (stay) " << "Stay in your current user-profile" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::groups_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - groups [options]" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Show all groups in the system" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Options:" << std::endl << "-v (verbose) " << "Detailed output: outputs group_id, groupname, creater_id, creation time" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::mygroups_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage" << std::endl;
    std::cout << " - mygroups [options]" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Show all user-groups in the system" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Options:" << std::endl << "-v (verbose) " << "Detailed output: outputs group_id, groupname, creater_id" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::groupcount_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - groupcount" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Description:" << std::endl;
	std::cout << "Show the number of groups on the vfs (without superuser's group)" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::newgroup_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
	std::cout << " - newgroup <groupname>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Create new group and add active user in it" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::addtogroup_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - addtogroup <username> <groupname>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Add the specified user to the specified group" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::rmmefromgroup_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - rmmefromgroup <groupname>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Remove the active user from the specified group, if possible" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::rmgroup_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - rmgroup <groupname>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Remove the specified user group, if possible" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::mkdir_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - mkdir <path> <dirname>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Create a virtual directory along the specified path, if specified. Otherwise, in the current directory" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::mkfile_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - mkfile <path> <filename> <content>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Create a virtual regular file along the specified path, if specified. Otherwise, in the current directory" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::rmdir_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - rmdir [options] <path> <dirname>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Removes a virtual directory along the specified path, if specified. Otherwise, searches in the current directory" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Options:" << std::endl << "-r (recursive) " << "Will recursively delete a directory and all its contents" << 
	std::endl << "(normally rm will not delete directories, while rmdir will only delete empty directories)" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::rmfile_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - rmfile <path> <dirname>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Remove a virtual regular file along the specified path, if specified. Otherwise, searches in the current directory" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::ls_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - ls [options]" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Show internal directory files" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Options:" << std::endl << 
	"-v (verbose) " << "Detailed output: outputs file_permissions, size(in bytes), creation_time, filename" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::cd_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - cd <path>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Move between directories" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::find_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - find [option1] [option2] <name_pattern>" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Search for a file or directory" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Options:" << std::endl << 
	"[option1]:" << std::endl << 
	"  -d (directory) " << "Search only directories" << std::endl <<
	"  -f (file) " << "Search only regular files" << std::endl <<
	"  -a (all) " << "Search files of all types" << std::endl <<
	"[option2]:" << std::endl << 
	"  -r (root) " << "Search in entire vfs starting from the root directory (recursive in all vfs)" << std::endl <<
	"  -c (current) " << "Search only in current directory" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::pwd_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - pwd" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Show the name of the current working directory" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::tree_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - tree [option1] [option2]" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Show tree view of vfs" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Options:" << std::endl << 
	"[option1]:" << std::endl << 
	"  -r (root) " << "Show tree view of vfs, start from root" << std::endl <<
	"  -c (current) " << "Show tree view of vfs, start from current directory" << std::endl <<
	"[option2]:" << std::endl << 
	"  -o (one) " << "Show tree view of vfs for only one level of nesting" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::clear_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - clear" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Clear the screen" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::help_help() const noexcept{
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - help" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "Show information on built-in commands" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

void View::exit_help() const noexcept{ 
	std::cout << "- - - - - - - - - -" << std::endl;
	std::cout << "Usage:" << std::endl;
    std::cout << " - exit" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl; 
	std::cout << "Description:" << std::endl;
	std::cout << "End the program" << std::endl;
	std::cout << "- - - - - - - - - -" << std::endl;
}

}