#include "SortView.hpp"
#include "Generator.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <random>
#include <filesystem>
#include <fstream>

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
    std::cout << "2. Remove the received <Transaction> data file" << std::endl;
    std::cout << "3. Remove the sorted <Transaction> data file" << std::endl;
    std::cout << "4. Sort the received data file (this action will create a sorted copy of the original data file)" << std::endl;
    std::cout << "5. Back" << std::endl;
    std::cout << "Enter your choice (1-5):" << std::endl;
    for (int i = 0; i < BAD_ATTEMPTS_COUNT; ++i){
        int result = read_num(true);
        if (result != -1 && result < 6){
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
    if (sequence.size() != 50){
        std::cerr << "Error: visualization of sorts works well if the sequence has 50 elements. Fill in the sequence of up to 50 elements" << std::endl;
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

bool SortView::huge_data_mode_warning() const noexcept {
    std::cout << "Warning! This program module will work with data sizes 1-5 GB. Make sure you have enough disk space" << std::endl;
    std::cout << "Continue? [y/n]" << std::endl;
    
    std::string input;
    std::cin >> input;

    if (!input.empty()) {
        input[0] = std::tolower(input[0]);
    }


    if (input == "y") {
        return true;
    }
    else if (input == "n") {
        return false;
    } 
    else {
        std::cerr << "Invalid input" << std::endl;
        return false;
    }
}

void SortView::huge_data_mode_generate_data() const noexcept {
    if(huge_data_mode_warning()){
        std::filesystem::path file_path_str(std::string(EXECUTABLE_PATH) + "/Transactions/origin_data.txt");
        if (std::filesystem::exists(file_path_str)) {
            std::filesystem::remove(file_path_str);
        }

        std::ofstream file(file_path_str);
        std::cout << "Enter the size of the generated data (1/2/3/4/5 GB):" << std::endl;
        int size_in_gb = -1;
        for (int i = 0; i < BAD_ATTEMPTS_COUNT; ++i){
            size_in_gb = read_num(true);
            if (size_in_gb != -1 && size_in_gb < 6){
                break;
            }
        }
        if (size_in_gb == -1){
            return;
        }
        generate_transactions_to_file(static_cast<long long>(size_in_gb) * 1024 * 1024 * 1024, file_path_str.string());
    }
}

void SortView::huge_data_mode_clear_data(std::string file_path_str) const noexcept {
    std::filesystem::path file_path(file_path_str);
    
    if (!std::filesystem::exists(file_path)){
        std::cerr << "File: " << file_path_str << " Does not exist" << std::endl;
        return;
    }

    std::cout << "Are you sure you want to delete the received data file? [y/n]" << std::endl;
    
    std::string input;
    std::cin >> input;

    if (!input.empty()) {
        input[0] = std::tolower(input[0]);
    }

    bool flag = false; 

    if (input == "y") {
        flag = true;
    }
    else if (input == "n") {
        flag = false;
    } 
    else {
        std::cerr << "Invalid input" << std::endl;
        flag = false;
    }
    
    if (!flag){
        return;
    }
       
    if (std::filesystem::is_regular_file(file_path)){
        try{
            if (std::filesystem::remove(file_path)) {
                std::cout << "File: " << file_path_str << " deleted successfully." << std::endl;
            } 
            else {
                std::cerr << "Error deleting file." << std::endl;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Exception deleting file: " <<e.what() << std::endl;
        }
    }
    else{
        std::cerr << "Error: " << file_path_str << " is not a file" << std::endl;
    }
}

sequence::SmartSequence<Transaction> SortView::read_transaction_block(std::ifstream& input_file) const noexcept {
    sequence::SmartSequence<Transaction> transaction_block;
    std::string line;
    for(size_t i = 0; i < BLOCK_SIZE; ++i){
        if (std::getline(input_file, line)) {
            transaction_block.push_back(from_string(line));
        }
        else {
            break;
        }
    }
    
    return transaction_block;
}

void SortView::write_transaction_block(std::ofstream& output_file, const sequence::SmartSequence<Transaction>& block) const noexcept {
    for (size_t i = 0; i < block.size(); ++i) {
        output_file << to_string(block.get(i)) << std::endl;
    }
}

void SortView::merge_sorted_files(const std::string& file_path_1, const std::string& file_path_2, const std::string& output_path) noexcept {
    std::ifstream file1(file_path_1);
    std::ifstream file2(file_path_2);
    std::ofstream output_file(output_path);

    if (!file1.is_open() || !file2.is_open() || !output_file.is_open()) {
        std::cerr << "Error opening files for merge." << std::endl;
        return;
    }

    std::string line1, line2;
    bool f1 = static_cast<bool>(std::getline(file1, line1));
    bool f2 = static_cast<bool>(std::getline(file2, line2));

    while (f1 || f2) {
        if(f1 && f2) {
            const Transaction t1 = from_string(line1);
            const Transaction t2 = from_string(line2);
            switch (selected_comparator){
            case 1:
                if (cmp_by_one(t1, t2) <= 0) {
                    output_file << line1 << std::endl;
                    f1 = static_cast<bool>(std::getline(file1, line1));
                } 
                else {
                    output_file << line2 << std::endl;
                    f2 = static_cast<bool>(std::getline(file2, line2));
                }
                break;
            case 2:
                if (cmp_by_three(t1, t2) <= 0) {
                    output_file << line1 << std::endl;
                    f1 = static_cast<bool>(std::getline(file1, line1));
                } 
                else {
                    output_file << line2 << std::endl;
                    f2 = static_cast<bool>(std::getline(file2, line2));
                }
                break;
            case 3:
                if (cmp_by_five(t1, t2) <= 0) {
                    output_file << line1 << std::endl;
                    f1 = static_cast<bool>(std::getline(file1, line1));
                } 
                else {
                    output_file << line2 << std::endl;
                    f2 = static_cast<bool>(std::getline(file2, line2));
                }
                break;
            default:
                break;
            }
        } 
        else if (f1) {
            output_file << line1 << std::endl;
            f1 = static_cast<bool>(std::getline(file1, line1));
        } 
        else if (f2) {
            output_file << line2 << std::endl;
            f2 = static_cast<bool>(std::getline(file2, line2));
        }
    }

    file1.close();
    file2.close();
    output_file.close();
}

void SortView::huge_data_mode_sort_data(const std::string& input_file_path, const std::string& output_file_path) noexcept {
    std::ifstream input_file(input_file_path);
    if (!input_file.is_open()) {
        std::cerr << "Error: Unable to open input file." << std::endl;
        return;
    }

    QuickSort<Transaction> quicksort;

    sequence::SmartSequence<std::string> temp_files;
    int file_counter = 0;

    while (input_file.good()) {
        std::string temp_file_name = "temp_file_" + std::to_string(file_counter++) + ".txt";
        tmn::sequence::SmartSequence<Transaction> block = read_transaction_block(input_file);
        if (block.size() > 0) {
            quicksort(block);
            
            std::ofstream temp_file(temp_file_name);
            if (temp_file.is_open()) {
                write_transaction_block(temp_file, block);
                temp_file.close();
                temp_files.push_back(temp_file_name);
            }
            else{
                std::cerr << "Error creating temp file" << std::endl; return ;
            }
        }
        else {
            break;
        }
    }

    input_file.close();
    while (temp_files.size() > 1) {
        std::string file1 = temp_files.back();
        temp_files.pop_back();
        std::string file2 = temp_files.back();
        temp_files.pop_back();
        std::string merged_file = "merged_file_" + std::to_string(file_counter++) + ".txt";
        merge_sorted_files(file1, file2, merged_file);
        temp_files.push_back(merged_file);
        remove(file1.c_str());
        remove(file2.c_str());
    }

    if (!temp_files.empty()) {
        std::ifstream final_sorted_file(temp_files[0]);
        std::ofstream output_file(output_file_path);
        
        if (final_sorted_file.is_open() && output_file.is_open()) {
            std::string line;
            while(std::getline(final_sorted_file, line)){
                output_file << line << std::endl;
            }
            final_sorted_file.close();
            output_file.close();
        }
        else{
            std::cerr << "Error  copying final file or creating output file" << std::endl;return;
        }

        remove(temp_files[0].c_str());
    }
}


int SortView::huge_data_mode_choosing_comparator() noexcept {
    std::cout << "1. Comparator by 1 arg: by dates" << std::endl;
    std::cout << "2. Comparator by 3 args: by amount & date & location" << std::endl;
    std::cout << "3. Comparator by 5 args: by payment & currency & amount & tax & items" << std::endl;
    std::cout << "4. Back" << std::endl;
    std::cout << "Enter your choice (1-4):" << std::endl;
    for (int i = 0; i < BAD_ATTEMPTS_COUNT; ++i){
        int result = read_num(true);
        if (result != -1 && result < 5){
            if (result > 0 && result < 4){
                selected_comparator = result;
            }
            return result;
        }
    }
    return -1;
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
                break;
            }
            case 2 : {
                bool flag = true;
                while(flag) {
                    switch (huge_data_menu()) {
                        case 1 : {
                            huge_data_mode_generate_data();
                            break;
                        }
                        case 2 : {
                            huge_data_mode_clear_data(std::string(EXECUTABLE_PATH) + "/Transactions/origin_data.txt");
                            break;
                        }
                        case 3 : {
                            huge_data_mode_clear_data(std::string(EXECUTABLE_PATH) + "/Transactions/sorted_data.txt");
                            break;
                        }
                        case 4 : {
                            int choosen_cmp = huge_data_mode_choosing_comparator();
                            if (choosen_cmp > 0 && choosen_cmp < 4){
                                huge_data_mode_sort_data(
                                    std::string(EXECUTABLE_PATH) + "/Transactions/origin_data.txt",
                                    std::string(EXECUTABLE_PATH) + "/Transactions/sorted_data.txt"
                                );
                            }
                            break;
                        }
                        default : {
                            flag = false;
                            break;
                        }
                    }
                }
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