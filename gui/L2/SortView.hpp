#pragma once

#include "../../include/Sort/Sort.hpp"
#include "Transaction.hpp"

namespace tmn {
namespace sort {

int cmp_by_dates(const Transaction& a, const Transaction& b);
int cmp_by_amount_date_location(const Transaction& a, const Transaction& b);
int cmp_by_payment_currency_amount_tax_items(const Transaction& a, const Transaction& b);

class SortView {
private:
    sequence::SmartSequence<int> sequence;
    tmn::Function<int(const&, const&)> cmp_by_one(cmp_by_transaction_id); // 0
    tmn::Function<int(const&, const&)> cmp_by_three(cmp_by_amount_date_location); // 1
    tmn::Function<int(const&, const&)> cmp_by_five(cmp_by_payment_currency_amount_tax_items); // 2
    int selected_comparator = 0;
public:
    void run();
private:
    void general_about() const noexcept;
    void view_mode_about() const noexcept;
    void huge_data_mode_about() const noexcept;

    void main_menu() const noexcept;
    void view_mode_menu() const noexcept;
    void huge_data_menu() const noexcept;

    void view_mode_push_back() const noexcept;
    void view_mode_clear() const noexcept;

    void huge_data_mode_warning() const noexcept;
    void huge_data_mode_generate_data() const noexcept;
    void huge_data_mode_choosing_comparator() const noexcept;
    void huge_data_mode_sort_data() const noexcept;
};

}
}