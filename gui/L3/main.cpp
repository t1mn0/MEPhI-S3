#include "../../include/VFS/VFSController.hpp"

tmn_vfs::User AuthorizationScreen();
void MainScreen(tmn_vfs::VirtualFileSystem& vfs);

int main(){
    auto user = AuthorizationScreen();
    auto vfs = tmn_vfs::VirtualFileSystem::Init(user);
    MainScreen(vfs);
}