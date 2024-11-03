#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
#include <vector>      // for vector
#include <algorithm>
#include <cctype>
#include <cmath> 
 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/dom/canvas.hpp"  // for Canvas
#include "ftxui/util/ref.hpp"  // for Ref
#include "ftxui/component/event.hpp"               // for Event
#include "ftxui/component/mouse.hpp"               // for Mouse
#include "ftxui/screen/color.hpp"
#include <ftxui/screen/screen.hpp>
#include "ftxui/component/mouse.hpp"  


// bool IsValidUsername(const std::string& username) {
//   if (username.length() < 3 || username.length() > 20) {
//     return false;
//   }

//   for (char c : username) {
//     if (!isalnum(c) && c != '-' && c != '_') {
//       return false;
//     }
//     if (isalpha(c) && !islower(c) && !isupper(c)){
//         return false;
//     }
//   }
//   return true;
// }

// bool IsValidFullname(const std::string& fullname) {
//   if (fullname.length() < 3 || fullname.length() > 20) {
//     return false;
//   }

//   for (char c : fullname) {
//     if (isdigit(c)) {
//       return false;
//     }
//     if (!isalpha(c) && c != ' ') {
//       return false;
//     }
//     if (isalpha(c) && !islower(c) && !isupper(c)){
//         return false;
//     }
//   }

//   return true;
// }
 
using namespace ftxui;

Component Wrap(std::string name, Component component) {
  return Renderer(component, [name, component] {
    return hbox({
               text(name) | size(WIDTH, EQUAL, 8),
               separator(),
               component->Render() | xflex,
           }) |
           xflex;
  });
}

int main() {
  int mouse_x = 0;
  int mouse_y = 0;

  auto screen = ScreenInteractive::Fullscreen();

  std::string username = "";
  std::string fullname = "";
  std::string password_hash = "";

  Component input_username = Input(&username, "username");
  Component input_fullname = Input(&fullname, "fullname");

  InputOption password_option;
  password_option.password = true;
  Component input_password_hash = Input(&password_hash, "password", password_option);

  std::string button_label = "Yes. Authorization, please";
  std::function<void()> on_button_clicked_;
  auto button = Button(&button_label, screen.ExitLoopClosure());
  //button = Wrap("", button);

    auto component = Container::Vertical({
      input_username,
      input_fullname,
      input_password_hash,
      button,
  });

  auto renderer = Renderer(component, [&] { //return component->Render() | border; 
    return vbox({
               window(text(" Username : ") | hcenter | bold, input_username->Render()) | color(Color::DeepSkyBlue1),
               window(text(" Fullname : ") | hcenter | bold, input_fullname->Render()) | color(Color::DeepSkyBlue1),
               window(text(" Password : ") | hcenter | bold, input_password_hash->Render()) | color(Color::DeepSkyBlue1),
               separatorHeavy(),
               text("Hello " + username + "! My name is Alexa. I am guide person in this program. And i know who are you! " 
                    + fullname + ", isn't it?") | hcenter | vcenter | color(Color::SlateBlue1) | size(HEIGHT, EQUAL, 10),
               separatorHeavy(),
               window(text("Ready to start?"), button->Render()),
               separatorHeavy(),
           }) | borderRounded;
  });
 
  screen.Loop(renderer);
}