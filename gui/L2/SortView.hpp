#pragma once

#include "../../include/Sort/Sort.hpp"
#include "Transaction.hpp"
#include "VisualSorters.hpp"

namespace tmn {
namespace sort {

inline int BAD_ATTEMPTS_COUNT = 5;

int cmp_by_dates(const Transaction& a, const Transaction& b);
int cmp_by_amount_date_location(const Transaction& a, const Transaction& b);
int cmp_by_payment_currency_amount_tax_items(const Transaction& a, const Transaction& b);

class SortView {
private: 
    mutable sequence::SmartSequence<int> sequence;
    Function<int(const Transaction&, const Transaction&)> cmp_by_one = cmp_by_dates; // 0
    Function<int(const Transaction&, const Transaction&)> cmp_by_three = cmp_by_amount_date_location; // 1
    Function<int(const Transaction&, const Transaction&)> cmp_by_five = cmp_by_payment_currency_amount_tax_items; // 2
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
    int huge_data_menu() const noexcept;
    int sort_menu() const noexcept;

    void view_mode_push_back() const noexcept;
    void view_mode_pop_back() const noexcept;
    void view_mode_clear() const noexcept;
    void view_mode_fill_random() const noexcept;
    void visualize_sorting() const noexcept;

    void huge_data_mode_warning() const noexcept;
    void huge_data_mode_generate_data() const noexcept;
    void huge_data_mode_choosing_comparator() const noexcept;
    void huge_data_mode_sort_data() const noexcept;
};

}
}