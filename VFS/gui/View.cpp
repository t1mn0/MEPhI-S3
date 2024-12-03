#include <iostream>
#include <string>

#include "View.hpp"

namespace tmn_vfs {

View::View() {
    vfs = VirtualFileSystem::Init();
}

View::View(std::string password) {
    vfs = VirtualFileSystem::Init(password);
}

void View::run() {
    bool exit = false;
    while (!exit) {
        std::string line;
        std::cout << "(vfs) > ";
        std::getline(std::cin, line);

        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if (command == "exit") {
            exit = true;
        }
        else if (command == "users") {
            std::string verbose_option;
            iss >> verbose_option;
            users(verbose_option == "-v");
        } 
        else if (command == "whoami") {
            std::string verbose_option;
            iss >> verbose_option;
            whoami(verbose_option == "-v");
        } 
        else if (command == "usercount") {
            usercount();
        } 
        else if (command == "login") {
            std::string username, password;
            iss >> username >> password;
            login(username, std::to_string(std::hash<std::string>{}(password)));
        } 
        else if (command == "newuser") {
            std::string stay_option, username, fullname_with_password;
            iss >> stay_option;

            if (stay_option != "-st"){
                username = stay_option;
            }
            else{
                iss >> username;
            }

            std::getline (iss >> std::ws, fullname_with_password);

            size_t last_space = fullname_with_password.rfind(' ');
            std::string fullname, password;
            if (last_space != std::string::npos) {
                fullname = fullname_with_password.substr(0, last_space);
                password = fullname_with_password.substr(last_space + 1);
            } 
            else {
                std::cerr << "Error: Password not specified!" << std::endl;
                continue;
            }

            if (stay_option != "-st"){
                std::cerr << "Warning: stay option ignored, you'll be automatically login" << std::endl;
            }
            
            newuser(username, fullname, std::to_string(std::hash<std::string>{}(password)), stay_option == "-st");
        } 
        else if (command == "groups") {
            std::string verbose_option;
            iss >> verbose_option;
            groups(verbose_option == "-v");
        } 
        else if (command == "mygroups") {
            std::string verbose_option;
            iss >> verbose_option;
            mygroups(verbose_option == "-v");
        } 
        else if (command == "groupcount") {
            groupcount();
        }
        else if (command == "newgroup") {
            std::string groupname;
            iss >> groupname;
            newgroup(groupname);
        } 
        else if (command == "addtogroup") {
            std::string username, groupname;
            iss >> username >> groupname;
            addtogroup(username, groupname);
        } 
        else if (command == "rmmefromgroup") {
            std::string groupname;
            iss >> groupname;
            rmmefromgroup(groupname);
        } 
        else if (command == "rmgroup") {
            std::string groupname;
            iss >> groupname;
            rmgroup(groupname);
        } 
        else if (command == "mkdir") {
            std::string path, dirname;
            iss >> path >> dirname;
            if (dirname.empty()){
                dirname = path;
                path = "";
            }
            mkdir(dirname, path);
        } 
        else if (command == "mkfile") {
            std::string path, filename, physical_file_path;
            iss >> path >> filename >> physical_file_path;

            if (path == "-c"){
                mkfile(filename, std::filesystem::path(physical_file_path));
            }
            else{
                mkfile(filename, std::filesystem::path(physical_file_path), path);
            }
        } 
        // else if (command == "addcontent")
        else if (command == "setgroup") {
            std::string filename, groupname;
            iss >> filename >> groupname;

            setgroup(filename, groupname);
        }
        else if (command == "renamefile"){
            std::string old_filename, new_filename;
            iss >> old_filename >> new_filename;

            renamefile(old_filename, new_filename);
        }
        else if (command == "cat") {
            std::string path, filename;
            iss >> path >> filename;
            if (filename.empty()){
                filename = path;
                path = "";
            }
            cat(filename, path);
        } 
        else if (command == "ls") {
            std::string verbose_option;
            iss >> verbose_option;
            ls(verbose_option == "-v");
        } 
        else if (command == "cd") {
            std::string path;
            iss >> path;
            cd(path);
        } 
        else if (command == "find") {
            std::string filetype, where, namepattern;
            iss >> filetype >> where >> namepattern;
            if (filetype == "-c" || filetype == "-r"){
                namepattern = where;
                where = filetype;
                find(0, where == "-c", namepattern);
            }
            else if (filetype == "-a") {
                if (where != "-c" && where != "-r"){
                    namepattern = where;
                    where = "-c";
                }
                find(0, where == "-c", namepattern);
            }
            else if (filetype == "-f") {
                if (where != "-c" && where != "-r"){
                    namepattern = where;
                    where = "-c";
                }
                find(1, where == "-c", namepattern);
            }
            else if (filetype == "-d") {
                if (where != "-c" && where != "-r"){
                    namepattern = where;
                    where = "-c";
                }
                find(2, where == "-c", namepattern);
            }
            else{
                namepattern = filetype;
                find(0, false, namepattern);
            }
        } 
        else if (command == "pwd") {
            pwd();
        }
        else if (command == "clear") {
            clear();
        }
        else if (command == "help") {
            std::string command_name;
            iss >> command_name;
            if (command_name.empty()){
                global_help();
            }
            else if (command_name == "users"){
                users_help();
            }
            else if (command_name == "whoami"){
                whoami_help();
            }
            else if (command_name == "usercount"){
                usercount_help();
            }
            else if (command_name == "login"){
                login_help();
            }
            else if (command_name == "newuser"){
                newuser_help();
            }
            else if (command_name == "groups"){
                groups_help();
            }
            else if (command_name == "mygroups"){
                mygroups_help();
            }
            else if (command_name == "groupcount"){
                groupcount_help();
            }
            else if (command_name == "newgroup"){
                newgroup_help();
            }
            else if (command_name == "addtogroup"){
                addtogroup_help();
            }
            else if (command_name == "rmgroup"){
                rmgroup_help();
            }
            else if (command_name == "mkdir"){
                mkdir_help();
            }
            else if (command_name == "mkfile"){
                mkfile_help();
            }
            else if (command_name == "rmdir"){
                rmdir_help();
            }
            else if (command_name == "rmfile"){
                rmfile_help();
            }
            else if (command_name == "cat"){
                cat_help();
            }
            else if (command_name == "ls"){
                ls_help();
            }
            else if (command_name == "cd"){
                cd_help();
            }
            else if (command_name == "find"){
                find_help();
            }
            else if (command_name == "pwd"){
                pwd_help();
            }
            else if (command_name == "clear"){
                clear_help();
            }
            else if (command_name == "help"){
                help_help();
            }
            else if (command_name == "exit"){
                exit_help();
            }

        }
        else {
            std::cout << "Wrong command" << std::endl;
        }
    }
}
  
void View::pwd() const noexcept {
    std::cout << vfs.PWD() << std::endl;
}

void View::clear() const noexcept {
    std::cout << "\x1B[2J\x1B[H" << std::endl;
}

}