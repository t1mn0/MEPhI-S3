#pragma once

#include "../../include/Sort/Sort.hpp"
#include "Transaction.hpp"
#include "VisualSorters.hpp"

#include <fstream>
#include <filesystem>

namespace tmn {
namespace sort {

inline int BAD_ATTEMPTS_COUNT = 5;
inline int BLOCK_SIZE = 5000;

int cmp_by_amount_date_location(const Transaction& a, const Transaction& b);
int cmp_by_payment_currency_amount_tax_items(const Transaction& a, const Transaction& b);

class SortView {
private: 
    mutable sequence::SmartSequence<int> sequence;
    Function<int(const Transaction&, const Transaction&)> cmp_by_three = cmp_by_amount_date_location; // 0
    Function<int(const Transaction&, const Transaction&)> cmp_by_five = cmp_by_payment_currency_amount_tax_items; // 1
    int selected_comparator = 0;
    sort::VisualHeapSort heapsort;
    sort::VisualQuickSort quicksort;
    sort::VisualShellSort shellsort;

public:
    void run();
    
private:
    void fill_sequence_random_50() const noexcept;
    int read_num(bool only_positive = false) const;

    int main_menu() const noexcept;
    int view_mode_menu() const noexcept;
    int large_data_menu() const noexcept;
    int sort_menu() const noexcept;

    void view_mode_push_back() const noexcept;
    void view_mode_pop_back() const noexcept;
    void view_mode_clear() const noexcept;
    void view_mode_fill_random() const noexcept;
    void view_mode_visualize_sorting() const noexcept;

    bool large_data_mode_warning() const noexcept;
    void large_data_mode_generate_data() const noexcept;
    void large_data_mode_clear_data(std::string path) const noexcept;
    int large_data_mode_choosing_comparator() noexcept;

    sequence::SmartSequence<Transaction> read_transaction_block(std::ifstream& input_file) const noexcept;
    void write_transaction_block(std::ofstream& output_file, const sequence::SmartSequence<Transaction>& block) const noexcept;
    void merge_sorted_files(const std::string& file_path_1, const std::string& file_path_2, const std::string& output_path) noexcept;
    void large_data_mode_sort_data(const std::string& input_file_path, const std::string& output_file_path) noexcept;
};  

}
}