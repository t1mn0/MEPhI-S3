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

using namespace ftxui;

tmn::Optional<tmn_vfs::User> NewUserScreen(tmn_vfs::VirtualFileSystem& vfs) {
    auto screen = ScreenInteractive::Fullscreen();
    std::string username = "";
    std::string fullname = "";
    std::string password = "";

    std::string creating_user_error = "";

    std::string button_label_create_user = "Create a new user and log in";
    std::string button_label_quit = "Quit";

    std::vector<std::string> toggle_roles = {
        "LOCAL",
        "GUEST",
    };

    int toggle_selected_role = 0;

    Component input_username = Input(&username, "username");
    Component input_fullname = Input(&fullname, "fullname");

    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "password", password_option);
    
    Component toggle_role = Toggle(&toggle_roles, &toggle_selected_role);

    auto button_create_user = Button(&button_label_create_user, [&] {
        tmn_vfs::VFSController::IsValidUsername(username) && 
        tmn_vfs::VFSController::IsValidFullname(fullname) && 
        !vfs.IsUserInSystem(username) && 
        password != "" ? screen.Exit() : tmn_vfs::VFSController::WrongAuthorization(creating_user_error);});

    auto button_quit = Button(&button_label_quit, [&] { screen.Exit() ;});

    auto component = Container::Vertical({
        input_username,
        input_fullname,
        input_password,
        toggle_role,
        button_create_user,
        button_quit,
    });

    auto renderer = Renderer(component, [&] {
        return vbox({
                vbox({
                    text("Creating new user...") | center | bold, 
                }),
                vbox({
                    vbox({
                        window(text(" Username ") | hcenter | bold, input_username->Render()) | color(Color::DeepSkyBlue1),
                        separator(),
                        window(text(" Fullname ") | hcenter | bold, input_fullname->Render()) | color(Color::DeepSkyBlue1),
                        separator(),
                        window(text(" Password ") | hcenter | bold, input_password->Render()) | color(Color::DeepSkyBlue1),
                    }),
                    separator(),
                    vbox({
                        window(text("Choose your role"), toggle_role->Render() | center) | color(Color::DeepSkyBlue1),
                    }), 
                    separatorHeavy(), 
                    vbox({ 
                        button_create_user->Render() | vcenter | color(Color::DeepSkyBlue1),
                    }),
                    separatorHeavy(), 
                    vbox({ 
                        text("By default, you will be created in the 'default' group.") | center | color(Color::SlateBlue1) | size(HEIGHT, EQUAL, 3),
                    }),
                }) | borderRounded | size(WIDTH, EQUAL, 70) | size(HEIGHT, EQUAL, 25) | center,
                vbox({ 
                    button_quit->Render() | vcenter | color(Color::Orchid),
                }),
                vbox({
                    text(creating_user_error) | center | bold | color(Color::Red3), 
                }),
            }) | center;
    });

    screen.Loop(renderer);

    if( !tmn_vfs::VFSController::IsValidUsername(username) || 
        !tmn_vfs::VFSController::IsValidFullname(fullname) || 
        password == ""){
            return tmn::Optional<tmn_vfs::User>();
        }

    tmn_vfs::User user;
    if (toggle_selected_role == 0){
        user = tmn_vfs::User(username, fullname, std::to_string(std::hash<std::string>{}(password)), tmn_vfs::UserStatus::LOCAL);
    }
    else{
        user = tmn_vfs::User(username, fullname, std::to_string(std::hash<std::string>{}(password)), tmn_vfs::UserStatus::GUEST);
    }

    return tmn::Optional<tmn_vfs::User>(user);
}