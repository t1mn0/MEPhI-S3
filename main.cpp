#include "VFS/gui/View.hpp"
#include "VFS/include/Utils.hpp"

#include <iostream>

int main(){
    // std::cout << tmn_vfs::FileNameMatch("file", "*") << std::endl;
    // std::cout << tmn_vfs::FileNameMatch("file", "   ") << std::endl;
    // std::cout << tmn_vfs::FileNameMatch("file.txt", "*.txt") << std::endl;
    // std::cout << tmn_vfs::FileNameMatch("file.txt", "*xt") << std::endl;
    // std::cout << tmn_vfs::FileNameMatch("file.txt", "f*") << std::endl;
    // std::cout << tmn_vfs::FileNameMatch("file.txt", "file*") << std::endl;
    // std::cout << tmn_vfs::FileNameMatch("file.txt", "**") << std::endl;
    tmn_vfs::View view;
    view.run();
    
    return 0;
}