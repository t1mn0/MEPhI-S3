#include <iostream>

#include "VFS/gui/View.hpp"
#include "VFS/include/VFS.hpp" // for check vfs in system 
#include "VFS/include/Utils.hpp" // for good password

#include <iostream>

int main(){
    if (!tmn_vfs::VirtualFileSystem::in_system()){
        std::string password;
        std::cout << "Enter the password for the root user" << std::endl; 
        std::cin >> password;
        getchar();

        while (!tmn_vfs::is_good_password(password)){
            std::cout << "Bad password" << std::endl; 
            std::cin >> password;
        }

        tmn_vfs::View view(password);
        view.run();
        return 0;
    }
    
    tmn_vfs::View view;
    view.run();
    
    return 0;
}