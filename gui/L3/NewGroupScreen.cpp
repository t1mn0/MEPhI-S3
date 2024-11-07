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

tmn::Optional<tmn_vfs::Group> NewGroupScreen(tmn_vfs::VirtualFileSystem& vfs) {
    auto screen = ScreenInteractive::Fullscreen();
    std::string groupname = "";
    std::string password = "";

    std::string creating_group_error = "";

    std::string button_label_create_group = "Create a new group";
    std::string button_label_quit = "Quit";

    bool button_is_pressed = false;
    
    Component input_groupname = Input(&groupname, "groupname");

    InputOption password_option;
    password_option.password = true;
    Component input_password = Input(&password, "password", password_option);

    auto button_create_group = Button(&button_label_create_group, [&] {
        if(tmn_vfs::VFSController::IsValidUsername(groupname) && 
            !vfs.IsGroupInSystem(groupname) && password != ""){
            screen.Exit();
            button_is_pressed = true;
        }    
        else{
            tmn_vfs::VFSController::WrongAuthorization(creating_group_error);
        };
    });

    auto button_quit = Button(&button_label_quit, [&] { screen.Exit() ;});

    auto component = Container::Vertical({
        input_groupname,
        input_password,
        button_create_group,
        button_quit,
    });

    auto renderer = Renderer(component, [&] {
        return vbox({
                vbox({
                    text("Creating new group...") | center | bold, 
                }),
                vbox({
                    vbox({
                        window(text(" Groupname ") | hcenter | bold, input_groupname->Render()) | color(Color::DeepSkyBlue1),
                        separator(),
                        window(text(" Password ") | hcenter | bold, input_password->Render()) | color(Color::DeepSkyBlue1),
                    }),
                    separator(),
                    separatorHeavy(), 
                    vbox({ 
                        button_create_group->Render() | vcenter | color(Color::DeepSkyBlue1),
                    }),
                    separatorHeavy(), 
                    vbox({ 
                        text("When you create a new group you are automatically added to it") | center | color(Color::SlateBlue1) | size(HEIGHT, EQUAL, 3),
                    }),
                }) | borderRounded | size(WIDTH, EQUAL, 70) | size(HEIGHT, EQUAL, 25) | center,
                vbox({ 
                    button_quit->Render() | vcenter | color(Color::Orchid),
                }),
                vbox({
                    text(creating_group_error) | center | bold | color(Color::Red3), 
                }),
            }) | center;
    });

    screen.Loop(renderer);

    if (button_is_pressed){
        if( !tmn_vfs::VFSController::IsValidUsername(groupname) || password == ""){
                return tmn::Optional<tmn_vfs::Group>();
            }

        tmn_vfs::Group group;
        group = tmn_vfs::Group(groupname, vfs.WhoUser(), std::to_string(std::hash<std::string>{}(password)));

        return tmn::Optional<tmn_vfs::Group>(group);
    }
    
    return tmn::Optional<tmn_vfs::Group>();
}