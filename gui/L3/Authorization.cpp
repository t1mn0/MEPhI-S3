#include <functional>  // for function
#include <memory>      // for shared_ptr, allocator, __shared_ptr_access
#include <string>      // for string, basic_string
 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"       // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp"  // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for text, hbox, separator, Element, operator|, vbox, border

using namespace ftxui;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Valid-checkers :

bool IsValidUsername(const std::string& username) {
  if (username.length() < 3 || username.length() > 20) {
    return false;
  }

  for (char c : username) {
    if (!isalnum(c) && c != '-' && c != '_') {
      return false;
    }
    if (isalpha(c) && !islower(c) && !isupper(c)){
        return false;
    }
  }
  return true;
}

bool IsValidFullname(const std::string& fullname) {
  if (fullname.length() < 3 || fullname.length() > 20) {
    return false;
  }

  for (char c : fullname) {
    if (isdigit(c)) {
      return false;
    }
    if (!isalpha(c) && c != ' ') {
      return false;
    }
    if (isalpha(c) && !islower(c) && !isupper(c)){
        return false;
    }
  }

  return true;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Main :

int main() {
  auto screen = ScreenInteractive::Fullscreen();
  std::string username = "";
  std::string fullname = "";
  std::string password_hash = "";

  std::string authorization_error = "";

  std::string button_label = "Yes. Authorization, please";

  // 1) Inputs
  Component input_username = Input(&username, "username");
  Component input_fullname = Input(&fullname, "fullname");

  InputOption password_option;
  password_option.password = true;
  Component input_password_hash = Input(&password_hash, "password", password_option);

  // TODO : password_hash = hash(password_hash);

  // 2) Button
  auto button = Button(&button_label, [&] {
    IsValidUsername(username) && IsValidFullname(fullname) && password_hash != "" ? authorization_error = "" : authorization_error = "Something wrong... Please, try again.";});

  auto component = Container::Vertical({
    input_username,
    input_fullname,
    input_password_hash,
    button,
  });

  auto renderer = Renderer(component, [&] {
    return vbox({
              vbox({
                text("MEPhI ICIS S3-L3") | center | bold, 
              }),
              vbox({
                vbox({
                  window(text(" Username ") | hcenter | bold, input_username->Render()) | color(Color::DeepSkyBlue1),
                  separator(),
                  window(text(" Fullname ") | hcenter | bold, input_fullname->Render()) | color(Color::DeepSkyBlue1),
                  separator(),
                  window(text(" Password ") | hcenter | bold, input_password_hash->Render()) | color(Color::DeepSkyBlue1),
                }),
                separatorHeavy(),
                vbox({
                  text("Hello " + username + "!") | center | color(Color::SlateBlue1) | size(HEIGHT, EQUAL, 3),
                  text("My name is Alexa. I am guide person in this program.") | center | color(Color::SlateBlue1) | size(HEIGHT, EQUAL, 2),
                  text("And i know who are you!" + fullname + ", isn't it?") | center | color(Color::SlateBlue1) | size(HEIGHT, EQUAL, 2),
                }),
                separatorHeavy(), 
                vbox({ 
                  window(text("Ready to start?"), button->Render()) | vcenter | color(Color::DeepSkyBlue1),
                }),
                separatorHeavy(), 
                vbox({ 
                  text("By default, you will be created in the 'default' group.") | center | color(Color::SlateBlue1) | size(HEIGHT, EQUAL, 3),
                }),
               }) | borderRounded | size(WIDTH, EQUAL, 70) | size(HEIGHT, EQUAL, 31) | center,
               //gauge(progress) | color(Color::SeaGreen1) | borderRounded,
               vbox({
                text(authorization_error) | center | bold | color(Color::Red3), 
               }),
           }) | center;
  });
 
  screen.Loop(renderer);
}

// ADD FILNALLY GET USERINFO AND FORM HIM