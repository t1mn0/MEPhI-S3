#include "SortView.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <random>

namespace tmn {
namespace sort {

void SortView::fill_sequence_random_50() const noexcept {
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(-50, 50);

    for (size_t i = 0; i < 50; ++i) {
        sequence.push_back(distribution(generator)); 
    }
}

int SortView::read_num(bool only_positive) const {
    std::string input_line;
    int result;
    std::cout << "> ";
    std::cin >> input_line;
    std::stringstream ss(input_line);
    if ((ss >> result) && ((only_positive && (result > 0)) || !only_positive)) {
        char leftover;
        if (ss >> leftover){
            std::cerr << "Error: Invalid input" << std::endl;
            return -1;
        }
        return result;
    }
    else{
        std::cerr << "Error: Invalid input" << std::endl;
    }
    if (!only_positive){
        throw;
    }
    return -1;
}

int SortView::main_menu() const noexcept {
    std::cout << "You can choose the mode you are interested in:" << std::endl;
    std::cout << "1. Visualization of sorts" << std::endl;
    std::cout << "2. Working with a large amount of data" << std::endl;
    std::cout << "3. Exit" << std::endl;
    std::cout << "Enter your choice (1-3):" << std::endl;
    for (int i = 0; i < BAD_ATTEMPTS_COUNT; ++i){
        int result = read_num(true);
        if (result != -1 && result < 4){
            return result;
        }
    }
    return -1;
}

int SortView::view_mode_menu() const noexcept {
    std::cout << "Sequence status:" << std::endl;
    std::cout << " - size : " << sequence.size() << std::endl;
    std::cout << " - capacity : " << sequence.capacity() << std::endl;
    std::cout << " - elements : ";
    for (size_t i = 0; i < sequence.size(); ++i){
        std::cout << sequence.get(i) << " "; 
    }
    std::cout << std::endl;

    std::cout << "1. Add an element to the end of the sequence (push_back)" << std::endl;
    std::cout << "2. Remove the element from the end of the sequence (pop_back)" << std::endl;
    std::cout << "3. Clear the sequence (clear)" << std::endl;
    std::cout << "4. Fill the sequence (only for empty sequence, random[-50, 50])" << std::endl;
    std::cout << "5. Visualize the sorting" << std::endl;
    std::cout << "6. Back" << std::endl;
    std::cout << "Enter your choice (1-6):" << std::endl;
    for (int i = 0; i < BAD_ATTEMPTS_COUNT; ++i){
        int result = read_num(true);
        if (result != -1 && result < 7){
            return result;
        }
    }
    return -1;
}

int SortView::huge_data_menu() const noexcept {
    std::cout << "1. Create a huge unordered array of <Transaction> type data and write it in file" << std::endl;
    std::cout << "2. Clear the received <Transaction> data file" << std::endl;
    std::cout << "3. Sort the received data file" << std::endl;
    std::cout << "4. Back" << std::endl;
    std::cout << "Enter your choice (1-4):" << std::endl;
    for (int i = 0; i < BAD_ATTEMPTS_COUNT; ++i){
        int result = read_num(true);
        if (result != -1 && result < 5){
            return result;
        }
    }
    return -1;
}

int SortView::sort_menu() const noexcept {
    std::cout << "1. HeapSort" << std::endl;
    std::cout << "2. QuickSort" << std::endl;
    std::cout << "3. ShellSort" << std::endl;
    std::cout << "4. Back" << std::endl;
    std::cout << "Enter your choice (1-4):" << std::endl;
    for (int i = 0; i < BAD_ATTEMPTS_COUNT; ++i){
        int result = read_num(true);
        if (result != -1 && result < 5){
            return result;
        }
    }
    return -1;
}

void SortView::view_mode_push_back() const noexcept {
    if (sequence.size() == 50){
        std::cerr << "Error: the maximum size of the sequence for visualization of sorting has been reached" << std::endl;
        return;
    }
    std::cout << "Enter an <int> number to add to the sequence:" << std::endl;
    int result;
    for (int i = 0; i < BAD_ATTEMPTS_COUNT; ++i){
        try {
            result = read_num();
            break;
        }
        catch(...){
            if (i == 4){
                return;
            }
        }
    }
    sequence.push_back(result);
}

void SortView::view_mode_pop_back() const noexcept {
    if (sequence.size() == 0){
        std::cerr << "Error: you cannot delete an element from an empty sequence" << std::endl;
        return;
    }
    sequence.pop_back();
}

void SortView::view_mode_clear() const noexcept {
    if (sequence.size() == 0){
        std::cerr << "Error: you cannot clear an empty sequence" << std::endl;
        return;
    }
    sequence.clear();
}

void SortView::view_mode_fill_random() const noexcept {
    if (sequence.size() != 0){
        std::cerr << "Error: you can only fill in an empty sequence. Clear the sequence" << std::endl;
        return;
    }
    fill_sequence_random_50();
}

void SortView::visualize_sorting() const noexcept {
    if (sequence.size() == 0){
        std::cerr << "Error: you cannot sort an empty sequence" << std::endl;
        return;
    }
    int selected_sorting = sort_menu();
    switch (selected_sorting) {
        case 1 : {
            heapsort(sequence);
            break;
        }
        case 2 : {
            quicksort(sequence);
            break;
        }
        case 3 : {
            shellsort(sequence);
            break;
        }
        default:
            break;
    }
}

void SortView::run() {
    while(true){
        switch (main_menu()) {
            case 1 : {
                bool flag = true;
                while(flag) {
                    switch (view_mode_menu()) {
                        case 1 : {
                            view_mode_push_back();
                            break;
                        }
                        case 2 : {
                            view_mode_pop_back();
                            break;
                        }
                        case 3 : {
                            view_mode_clear();
                            break;
                        }
                        case 4 : {
                            view_mode_fill_random();
                            break;
                        }
                        case 5 : {
                            visualize_sorting();
                            break;
                        }
                        default : {
                            flag = false;
                            break;
                        }
                    }
                }
            }
            case 2 : {
                break;
            }
            case 3 : {
                return;
            }
            default :
                return;
        }
    }

}

}
}