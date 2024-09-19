#include <ncurses.h>
#include <string>
#include <cstring>
#include <vector>
#include <memory>
#include <sstream>



struct IntObject {
    int* raw_pointer;
    std::shared_ptr<int> shared_pointer;
    std::unique_ptr<int> unique_pointer;
    std::string type;

    IntObject(int* ptr) : raw_pointer(ptr), shared_pointer(nullptr), unique_pointer(nullptr), type("Raw") {}
    IntObject(std::shared_ptr<int> ptr) : raw_pointer(nullptr), shared_pointer(ptr), unique_pointer(nullptr), type("Shared") {}
    IntObject(std::unique_ptr<int> ptr) : raw_pointer(nullptr), shared_pointer(nullptr), unique_pointer(std::move(ptr)), type("Unique") {}
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

    mvprintw(row - 2, 0, "'^Q' Quit, '^R' RawPtr, '^S' SharedPtr, '^U' UniquePtr, '^D' DoSome");
    attron(COLOR_PAIR(2));

    for (size_t i = 0; i < objects.size(); ++i) {
        std::stringstream ss;
        ss << (i == selected ? "> " : "  ") << objects[i].type << " Pointer: ";
        if (objects[i].type == "Raw") {
            ss << objects[i].raw_pointer << " (Value: " << *(objects[i].raw_pointer) << ")";
        } else if (objects[i].type == "Shared") {
            ss << objects[i].shared_pointer.get() << " (Value: " << *(objects[i].shared_pointer) << ", Count: " << objects[i].shared_pointer.use_count() << ")";
        } else if (objects[i].type == "Unique") {
            ss << objects[i].unique_pointer.get() << " (Value: " << *(objects[i].unique_pointer) << ")";
        }
        
        mvprintw(i + 1, 0, ss.str().c_str());
    }

    refresh();
}



void perform_operation(std::vector<IntObject>& objects, int selected) {
    clear();
    if (selected >= 0 && selected < objects.size()) {
        if (objects[selected].type == "Raw") {
            mvprintw(0, 0, "Raw Pointer:");
            mvprintw(1, 0, "Address: %p", objects[selected].raw_pointer);
            mvprintw(2, 0, "Value: %d", *(objects[selected].raw_pointer));
            mvprintw(4, 0, "Operations:");
            mvprintw(5, 0, "1. Delete from storage");

            int ch = getch();
            if (ch == '1') {
                delete objects[selected].raw_pointer;
                objects.erase(objects.begin() + selected);
            }
        } else if (objects[selected].type == "Shared") {
            mvprintw(0, 0, "Shared Pointer:");
            mvprintw(1, 0, "Address: %p", objects[selected].shared_pointer.get());
            mvprintw(2, 0, "Value: %d", *(objects[selected].shared_pointer));
            mvprintw(3, 0, "Count: %lu", objects[selected].shared_pointer.use_count());
            mvprintw(4, 0, "Operations:");
            mvprintw(5, 0, "1. Delete from storage");
            mvprintw(6, 0, "2. Swap with another shared_ptr");

            int ch = getch();
            if (ch == '1') {
                objects.erase(objects.begin() + selected);
            } 
            else if (ch == '2') {
                mvprintw(9, 0, "Select another shared_ptr to swap with:");
                for (size_t i = 0; i < objects.size(); ++i) {
                    if (objects[i].type == "Shared" && i != selected) {
                        mvprintw(10 + i, 0, "%lu: Shared Pointer", i + 1);
                    }
                }
                int swap_index = getch() - '1';
                if (swap_index >= 0 && swap_index < objects.size() && objects[swap_index].type == "Shared") {
                    std::swap(objects[selected].shared_pointer, objects[swap_index].shared_pointer);
                }
            }
        } else if (objects[selected].type == "Unique") {
            mvprintw(0, 0, "Unique Pointer:");
            mvprintw(1, 0, "Address: %p", objects[selected].unique_pointer.get());
            mvprintw(2, 0, "Value: %d", *(objects[selected].unique_pointer));
            mvprintw(4, 0, "Operations:");
            mvprintw(5, 0, "1. Delete from storage");
            mvprintw(6, 0, "2. Swap with another unique_ptr");

            int ch = getch();
            if (ch == '1') {
                objects.erase(objects.begin() + selected);
            } 
            else if (ch == '2') {
                mvprintw(9, 0, "Select another unique_ptr to swap with:");
                for (size_t i = 0; i < objects.size(); ++i) {
                    if (objects[i].type == "Unique" && i != selected) {
                        mvprintw(10 + i, 0, "%lu: Unique Pointer", i + 1);
                    }
                }
                int swap_index = getch() - '1';
                if (swap_index >= 0 && swap_index < objects.size() && objects[swap_index].type == "Unique") {
                    std::swap(objects[selected].unique_pointer, objects[swap_index].unique_pointer);
                }
            }
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
            if (selected >= 0 && selected < objects.size()) {
                if (objects[selected].type == "Raw") {
                    std::shared_ptr<int> shared_ptr(objects[selected].raw_pointer);
                    objects.emplace_back(shared_ptr);
                } else if (objects[selected].type == "Shared") {
                    objects.emplace_back(objects[selected].shared_pointer);
                }
            } else {
                std::shared_ptr<int> shared_ptr = std::make_shared<int>(rand() % 100);
                objects.emplace_back(shared_ptr);
            }
            first_command = false;
            break;
        }
        case 21: { // Ctrl+U
            if (selected >= 0 && selected < objects.size()) {
                if (objects[selected].type == "Raw") {
                    std::unique_ptr<int> unique_ptr(objects[selected].raw_pointer);
                    objects.emplace_back(std::move(unique_ptr));
                }
            } else {
                std::unique_ptr<int> unique_ptr = std::make_unique<int>(rand() % 100);
                objects.emplace_back(std::move(unique_ptr));
            }
            first_command = false;
            break;
        }
        case 4: { // Ctrl+D
            clear();
            mvprintw(0, 0, "Select an object to perform operations:");
            for (size_t i = 0; i < objects.size(); ++i) {
                std::stringstream ss;
                ss << (i + 1) << ". " << objects[i].type << " Pointer";
                mvprintw(i + 1, 0, ss.str().c_str());
            }
            refresh();
            int ch = getch();
            int selected_op = ch - '1';
            if (selected_op >= 0 && selected_op < objects.size()) {
                perform_operation(objects, selected_op);
            }
            first_command = false;
            break;
        }
        case KEY_UP:
            selected = (selected > 0) ? selected - 1 : selected;
            break;
        case KEY_DOWN:
            selected = (selected < objects.size() - 1) ? selected + 1 : selected;
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