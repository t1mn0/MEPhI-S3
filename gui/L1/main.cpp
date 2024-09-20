#include <ncurses.h>
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <sstream>

#include "../../include/SmartPtr/SharedPtr.hpp"
#include "../../include/SmartPtr/UniquePtr.hpp"



struct IntObject {
    int* raw_pointer;
    tmn_smart_ptr::SharedPtr<int> shared_pointer;
    tmn_smart_ptr::UniquePtr<int> unique_pointer;
    std::string type;

    IntObject(int* ptr) : raw_pointer(ptr), shared_pointer(nullptr), unique_pointer(nullptr), type("Raw") {}
    IntObject(tmn_smart_ptr::SharedPtr<int> ptr) : raw_pointer(nullptr), shared_pointer(ptr), unique_pointer(nullptr), type("Shared") {}
    IntObject(tmn_smart_ptr::UniquePtr<int> ptr) : raw_pointer(nullptr), shared_pointer(nullptr), unique_pointer(std::move(ptr)), type("Unique") {}
};



void initialize() {
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(1);

    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_BLACK, COLOR_WHITE);
        init_pair(2, COLOR_WHITE, COLOR_BLACK); 
    }
}



void draw_ui(const std::vector<IntObject>& objects, int selected, bool first_command) {

    int row, col;
    getmaxyx(stdscr, row, col);

    clear();

    attron(COLOR_PAIR(1));
    mvprintw(0, 0, "Use arrow keys to navigate.");

    const char *mesg = "MEPhI-3, L1";

    if (first_command) {
        mvwprintw(stdscr, row / 2, (col - strlen(mesg)) / 2, "%s", mesg);
    }

    mvprintw(row - 2, 0, "'^Q' Quit, '^R' RawPtr, '^S' SharedPtr, '^U' UniquePtr, '^D' CheckState");
    attron(COLOR_PAIR(2));

    for (size_t i = 0; i < objects.size(); ++i) {
        std::stringstream ss;
        ss << (i == static_cast<size_t>(selected) ? "> " : "  ") << objects[i].type << " Pointer: ";
        if (objects[i].type == "Raw") {
            ss << objects[i].raw_pointer << " (Value: " << *(objects[i].raw_pointer) << ")";
        } else if (objects[i].type == "Shared") {
            ss << objects[i].shared_pointer.get() << " (Value: " << (*(objects[i].shared_pointer)).value() << ", Count: " << objects[i].shared_pointer.use_count() << ")";
        } else if (objects[i].type == "Unique") {
            ss << objects[i].unique_pointer.get() << " (Value: " << (*(objects[i].unique_pointer)).value() << ")";
        }
        
        mvprintw(i + 1, 0, ss.str().c_str());
    }

    refresh();
}



void check_state(std::vector<IntObject>& objects, int selected) {
    clear();
    if (selected >= 0 && static_cast<size_t>(selected) < objects.size()) {
        if (objects[selected].type == "Raw") {
            mvprintw(0, 0, "Raw Pointer:");
            mvprintw(1, 0, "Address: %p", objects[selected].raw_pointer);
            mvprintw(2, 0, "Value: %d", *objects[selected].raw_pointer);
        } else if (objects[selected].type == "Shared") {
            mvprintw(0, 0, "Shared Pointer:");
            mvprintw(1, 0, "Address: %p", objects[selected].shared_pointer.get());
            mvprintw(2, 0, "Value: %d", (*(objects[selected].shared_pointer)).value());
            mvprintw(3, 0, "Count: %lu", objects[selected].shared_pointer.use_count());
        } else if (objects[selected].type == "Unique") {
            mvprintw(0, 0, "Unique Pointer:");
            mvprintw(1, 0, "Address: %p", objects[selected].unique_pointer.get());
            mvprintw(2, 0, "Value: %d", (*(objects[selected].unique_pointer)).value());
        }

        mvprintw(12, 0, "Press any key to return...");
        getch();
    }
}


void handle_keypress(int ch, int& selected, std::vector<IntObject>& objects, bool& first_command) {
    switch(ch) {
        case 17: // Ctrl+Q
            endwin();
            exit(0);
        case 18: { // Ctrl+R
            int* raw_ptr = new int(rand() % 100);
            objects.emplace_back(raw_ptr);
            first_command = false;
            break;
        }
        case 19: { // Ctrl+S
            if (selected >= 0 && static_cast<size_t>(selected) < objects.size()) {
                if (objects[selected].type == "Raw") {
                    tmn_smart_ptr::SharedPtr<int> SharedPtr(objects[selected].raw_pointer);
                    objects.emplace_back(SharedPtr);
                } else if (objects[selected].type == "Shared") {
                    objects.emplace_back(objects[selected].shared_pointer);
                }
            } else {
                tmn_smart_ptr::SharedPtr<int> SharedPtr = *(new tmn_smart_ptr::SharedPtr<int>(new int(rand() % 100)));
                objects.emplace_back(SharedPtr);
            }
            first_command = false;
            break;
        }
        case 21: { // Ctrl+U
            if (selected >= 0 && static_cast<size_t>(selected) < objects.size()) {
                if (objects[selected].type == "Raw") {
                    tmn_smart_ptr::UniquePtr<int> unique_ptr(objects[selected].raw_pointer);
                    objects.emplace_back(std::move(unique_ptr));
                }
            } else {
                tmn_smart_ptr::UniquePtr<int> unique_ptr = tmn_smart_ptr::make_unique<int>(rand() % 100);
                objects.emplace_back(std::move(unique_ptr));
            }
            first_command = false;
            break;
        }
        case 4: { // Ctrl+D
            clear();

            attron(COLOR_PAIR(1));
            mvprintw(0, 0, "Select an object to check state:");
            attron(COLOR_PAIR(2));
            for (size_t i = 0; i < objects.size(); ++i) {
                std::stringstream ss;
                ss << (i + 1) << ". " << objects[i].type << " Pointer: ";
                if (objects[i].type == "Raw") {
                    ss << objects[i].raw_pointer << " (Value: " << *(objects[i].raw_pointer) << ")";
                } else if (objects[i].type == "Shared") {
                    ss << objects[i].shared_pointer.get() << " (Value: " << (*(objects[i].shared_pointer)).value() << ", Count: " << objects[i].shared_pointer.use_count() << ")";
                } else if (objects[i].type == "Unique") {
                    ss << objects[i].unique_pointer.get() << " (Value: " << (*(objects[i].unique_pointer)).value() << ")";
                }
                mvprintw(i + 1, 0, ss.str().c_str());
            }
            refresh();
            int ch = getch();
            int selected_op = ch - '1';
            if (selected_op >= 0 && static_cast<size_t>(selected_op) < objects.size()) {
                check_state(objects, selected_op);
            }
            first_command = false;
            break;
        }
        case KEY_UP:
            selected = (selected > 0) ? selected - 1 : selected;
            break;
        case KEY_DOWN:
            selected = (static_cast<size_t>(selected) < objects.size() - 1) ? selected + 1 : selected;
            break;
        default:
            break;
    }
}



int main() {
    initialize();

    int selected = 0;
    bool first_command = true;
    std::vector<IntObject> objects;

    while (true) {
        draw_ui(objects, selected, first_command);
        int ch = getch();
        handle_keypress(ch, selected, objects, first_command);
    }

    endwin();
    return 0;
}