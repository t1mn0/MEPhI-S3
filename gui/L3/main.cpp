#include "../../include/VFS/VFSController.hpp"

tmn_vfs::User AuthorizationScreen();
void MainScreen(tmn_vfs::VirtualFileSystem& vfs);

// Не изменены нагрузочные тесты: использовать библиотеку C++ и писать со структурами, а не примитивами
// Можно доработать тесты по основному классу VFS
// Не доработан UI: 
//                  - просмотр файла / изменение файла / удаление файла
//                   \_----> с учетом прав доступа
//                  - меню пользователей + добавление суперпользователя и его контроля над пользователями
//                  - меню групп 
//                  - поисковик (результаты в футер)
// 


int main(){
    auto user = AuthorizationScreen();
    auto vfs = tmn_vfs::VirtualFileSystem::Init(user);
    MainScreen(vfs);
}