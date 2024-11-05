#include <memory>  // for shared_ptr, allocator, __shared_ptr_access
 
#include "ftxui/component/captured_mouse.hpp"  // for ftxui
#include "ftxui/component/component.hpp"  // for Renderer, ResizableSplitBottom, ResizableSplitLeft, ResizableSplitRight, ResizableSplitTop
#include "ftxui/component/component_base.hpp"      // for ComponentBase
#include "ftxui/component/screen_interactive.hpp"  // for ScreenInteractive
#include "ftxui/dom/elements.hpp"  // for Element, operator|, text, center, border
 
using namespace ftxui;
 
int main() {
    auto screen = ScreenInteractive::Fullscreen();
 
    auto middle = Renderer([] { return text("middle") | center; });
    auto left = Renderer([] { return text("Left") | center; });
    auto right = Renderer([] { return text("right") | center; });
 
    int left_size = 20;
    int right_size = 20;
 
    auto container = middle;
    container = ResizableSplitLeft(left, container, &left_size);
    container = ResizableSplitRight(right, container, &right_size);
 
    auto renderer = Renderer(container, [&] { // return container->Render() | border;
        return vbox({
                vbox({
                    text("MEPhI ICIS S3-L3 (mayb there is a place for user information here OR SEARCH!)") | center | bold, 
                }) | center | borderRounded | size(HEIGHT, EQUAL, 3),
                vbox({
                    container->Render() | border | size(HEIGHT, EQUAL, 35),
                }),
        }); 
        
    });
    
    screen.Loop(renderer);
}