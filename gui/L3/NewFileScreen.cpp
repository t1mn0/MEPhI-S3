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

tmn::Optional<tmn_vfs::FileInfo> NewFileScreen(tmn_vfs::VirtualFileSystem& vfs) {
    auto screen = ScreenInteractive::Fullscreen();
    std::string filename = "";
    
    std::vector<std::string> toggle_read_vec = {
        "Only me",
        "All",
        "Users in group (select)"
    };

    std::vector<std::string> toggle_write_vec = toggle_read_vec;
    std::vector<std::string> toggle_delete_vec = toggle_read_vec;


    std::vector<std::string> is_dir = {
        "File", "Directory"
    };

    int toggle_read_selected = 0;
    int toggle_write_selected = 0;
    int toggle_delete_selected = 0;
    int toggle_filetype_selected = 0;

    Component toggle_read = Toggle(&toggle_read_vec, &toggle_read_selected);
    Component toggle_write = Toggle(&toggle_write_vec, &toggle_write_selected);
    Component toggle_delete = Toggle(&toggle_delete_vec, &toggle_delete_selected);

    Component toggle_filetype = Toggle(&is_dir, &toggle_filetype_selected);

    std::string creating_file_error = "";

    std::string button_label_create_file = "Create a new file";
    std::string button_label_quit = "Quit";
    
    bool button_is_pressed = false;
    
    Component input_filename = Input(&filename, "filename");

    auto button_create_file = Button(&button_label_create_file, [&] {
        if (tmn_vfs::VFSController::IsValidFileName(filename)) {
            screen.Exit();
            button_is_pressed = true;
        } 
        else {
            tmn_vfs::VFSController::WrongCreatingFile(creating_file_error);
        }
    });
    
    auto button_quit = Button(&button_label_quit, [&] { screen.Exit() ;});

    auto component = Container::Vertical({
        input_filename,
        toggle_read,
        toggle_write,
        toggle_delete,
        toggle_filetype,
        button_create_file,
        button_quit,
    });

    auto renderer = Renderer(component, [&] {
        return vbox({
                vbox({
                    text("Create a new file in directory: " + vfs.PWD() + "...") | center | bold, 
                }),
                vbox({
                    vbox({
                        window(text(" Filename ") | hcenter | bold, input_filename->Render()) | color(Color::DeepSkyBlue1),
                        separator(),
                        vbox({
                            window(text("Read-permissions"), toggle_read->Render() | center) | color(Color::DeepSkyBlue1),
                            window(text("Write-permissions"), toggle_write->Render() | center) | color(Color::DeepSkyBlue1),
                            window(text("Delete-permissions"), toggle_delete->Render() | center) | color(Color::DeepSkyBlue1),
                        }),
                        separator(),
                        vbox({
                            toggle_filetype->Render() | center | color(Color::DeepSkyBlue1)
                        }),
                    }),
                    separatorHeavy(), 
                    vbox({ 
                        button_create_file->Render() | vcenter | color(Color::DeepSkyBlue1),
                    }),
                }) | borderRounded | size(WIDTH, EQUAL, 70) | size(HEIGHT, EQUAL, 25) | center,
                vbox({ 
                    button_quit->Render() | vcenter | color(Color::Orchid),
                }),
                vbox({
                    text(creating_file_error) | center | bold | color(Color::Red3), 
                }),
            }) | center;
    });

    screen.Loop(renderer);

    if(button_is_pressed){
        if( !tmn_vfs::VFSController::IsValidFileName(filename) || vfs.IsFileInSystem(vfs.PWD() + filename)){
            return tmn::Optional<tmn_vfs::FileInfo>();
        }

        tmn_vfs::FileInfo fi = tmn_vfs::FileInfo(toggle_filetype_selected, tmn_vfs::VFSController::GetFreeSector(), vfs.PWD() + "/" + filename, vfs.WhoUser());

        return tmn::Optional<tmn_vfs::FileInfo>(fi);
    }

    return tmn::Optional<tmn_vfs::FileInfo>();
}