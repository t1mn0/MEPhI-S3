#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border

#include "../../include/VFS/VFSController.hpp"
#include "../../include/VFS/Utils.hpp"

using namespace ftxui;

tmn::Optional<tmn_vfs::User> NewUserScreen(tmn_vfs::VirtualFileSystem& vfs);
tmn::Optional<tmn_vfs::Group> NewGroupScreen(tmn_vfs::VirtualFileSystem& vfs);
tmn::Optional<tmn_vfs::FileInfo> NewFileScreen(tmn_vfs::VirtualFileSystem& vfs);
 
void MainScreen(tmn_vfs::VirtualFileSystem& vfs) {
    auto screen = ScreenInteractive::Fullscreen();
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // SEARCHER
    std::string search_request = "";
    std::string button_label_search = "Search!";

    Component input_search = Input(&search_request, "          What you want to find?          ");

    auto button_search = Button(&button_label_search, screen.ExitLoopClosure());

    auto search_component = Container::Horizontal({
        input_search,
        button_search,
    });

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // USERINFO DROPDOWN
    std::string dd_userinfo_username = "Username: " + vfs.GetUserInfo(vfs.WhoUser()).value().GetUserName();
    std::string dd_userinfo_fullname = "Fullname: " + vfs.GetUserInfo(vfs.WhoUser()).value().GetFullName();
    std::string dd_userinfo_status = "Status: ";
    if (vfs.GetUserInfo(vfs.WhoUser()).value().GetStatus() == tmn_vfs::UserStatus::LOCAL){
        dd_userinfo_status += "LOCAL";
    }
    else{
        dd_userinfo_status += "GUEST";
    }
    std::string dd_userinfo_creation_time = "Creation time: " + vfs.GetUserInfo(vfs.WhoUser()).value().GetCreationTime();


    std::vector<std::string> dd_userinfo = {"UserInfo", dd_userinfo_username, dd_userinfo_fullname, dd_userinfo_status, dd_userinfo_creation_time};
    
    int selected_helper_userinfo = 0;
    
    auto layout_dd_userinfo = Container::Vertical({
        Dropdown(&dd_userinfo, &selected_helper_userinfo),
    });

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // USER-GROUPS DROPDOWN
    auto my_groups = vfs.GetAllUserGroups(vfs.WhoUser());

    std::vector<std::string> dd_helper_user_groups = {"My groups"};
    for (const auto& groupname : my_groups){
        dd_helper_user_groups.push_back(groupname);
    }
    
    int selected_helper_user_groups = 0;
    
    auto dd_user_groups = Dropdown(&dd_helper_user_groups, &selected_helper_user_groups);

    auto layout_dd_user_groups = Container::Vertical({
        dd_user_groups,
    });

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // ALL-GROUPS DROPDOWN
    tmn_sequence::ArraySequence<std::string> groups = vfs.GetAllGroups();


    std::vector<std::string> dd_helper_groups = {"All groups"};
    for (const auto& groupname : groups){
        dd_helper_groups.push_back(groupname);
    }
    
    int selected_helper_groups = 0;
    
    auto dd_groups = Dropdown(&dd_helper_groups, &selected_helper_groups);

    auto layout_dd_groups = Container::Vertical({
        dd_groups,
    });
 
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // BUTTONS CREATE_USER AND CREATE_GROUP

    tmn::Optional<tmn_vfs::User> optional_new_user;

    std::string button_label_create_user = "(+) New User";

    auto button_create_user = Button(&button_label_create_user, [&]{
        auto optional_new_user = NewUserScreen(vfs);
        if (optional_new_user.has_value()){
            vfs.AddUser(optional_new_user.value());
        }
    });

    std::string button_label_create_group = "(+) New Group";

    auto button_create_group = Button(&button_label_create_group, [&]{
        auto optional_new_group = NewGroupScreen(vfs);
        if (optional_new_group.has_value()){
            vfs.AddGroup(optional_new_group.value());
            dd_helper_groups.push_back(optional_new_group.value().GetGroupName());
            dd_helper_user_groups.push_back(optional_new_group.value().GetGroupName());
        }
    });

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // CURRENT_DIR DROPDOWN

    tmn_associative::HashSet<std::string> current_dir = vfs.CurrentDirContent();

    std::vector<std::string> dd_helper_currentdir = {"- FILES -"};
    for (const auto& filename : current_dir){
        dd_helper_currentdir.push_back(filename);
    }
    
    int selected_helper_files = 0;
    
    auto dd_files = Dropdown(&dd_helper_currentdir, &selected_helper_files);

    auto layout_dd_files = Container::Vertical({
        dd_files,
    });

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // BUTTONS CREATE_FILE AND CREATE_DIR

    std::string button_label_create_file = "(+) New File";

    auto button_create_file = Button(&button_label_create_file, [&]{
        auto optional_new_file = NewFileScreen(vfs);
        if (optional_new_file.has_value()){
            vfs.AddFile(optional_new_file.value());
            dd_helper_currentdir.push_back(tmn_vfs::CutFileName(optional_new_file.value().GetVirtualPath()));
        }
    });
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // RENDER
    auto middle = Renderer([] { return text("The main space for file contents") | center; });
    auto left = Renderer([&] { 
        return vbox({
            layout_dd_files->Render(),
            button_create_file->Render() | color(Color::DeepSkyBlue1),
        });
    });
    auto right = Renderer([&] { 
        return vbox({
            layout_dd_userinfo->Render(),
            layout_dd_user_groups->Render(),
            layout_dd_groups->Render(),
            button_create_user->Render() | color(Color::DeepSkyBlue1),
            button_create_group->Render() | color(Color::DeepSkyBlue1),
        });
    });
 
    int left_size = 20;
    int right_size = 20;
 
    auto container = middle;
    container = ResizableSplitRight(right, container, &right_size);
    container = ResizableSplitLeft(left, container, &left_size);

    auto main_container = Container::Vertical({
        search_component,
        container,
        layout_dd_files,
        button_create_file,
        layout_dd_userinfo,
        layout_dd_user_groups,
        layout_dd_groups,
        button_create_user,
        button_create_group
    });
 
    auto renderer = Renderer(main_container, [&] {
        return vbox({
                hbox({
                    border(input_search->Render() | center | size (WIDTH, EQUAL, 140)) | size (HEIGHT, EQUAL, 6) | color(Color::DeepSkyBlue1),
                    button_search->Render() | center | size (WIDTH, EQUAL, 15) | size (HEIGHT, EQUAL, 6) | color(Color::DeepSkyBlue1),
                }) | borderRounded | size(HEIGHT, EQUAL, 7),
                vbox({
                    container->Render() | border | size(HEIGHT, EQUAL, 28),
                }),
        }); 
        
    });
    
    screen.Loop(renderer);
}

// // СОЗДАНИЕ ПЕРВОГО ПОЛЬЗОВАТЕЛЯ - СТРАНИЦА 0
// // АВТОРИЗАЦИЯ (ВЫХОД НА СТРАНИЦУ НАЗАД ИЛИ НАЧАЛЬНУЮ СТРАНИЦУ) - СТРАНИЦА 1
// ОСНОВНАЯ ЧАСТЬ - СТРАНИЦА 2
// ОСНОВНЫЕ ДЕЙСТВИЯ С ФАЙЛАМИ: НИЖНЯЯ ЧАСТЬ ЛЕВОГО СПЛИТА. ВЕРХНЯЯ ДЛЯ ОТОБРАЖЕНИЯ ТЕКУЩЕЙ ДИРЕКТОРИИ
// // ПРАВЫЙ:      / ПРОСМОТРЕТЬ ИНФОРМАЦИЮ О ПОЛЬЗОВАТЕЛЕ 
//              / СМЕНИТЬ ПОЛЬЗОВАТЕЛЯ 
//              / СОЗДАТЬ ГРУППУ | ДОБАВИТЬСЯ В ГРУППУ | УДАЛИТЬСЯ ИЗ ГРУППЫ
// //             / СОЗДАТЬ ПОЛЬЗОВАТЕЛЯ 
//              / ЗАКОНЧИТЬ СЕССИЮ