#include <sstream>
#include <iomanip>
#include <ctime>

#include "Transaction.hpp"

namespace tmn {
namespace sort {

int cmp_by_amount_date_location(const Transaction& a, const Transaction& b) {
    if (a.amount != b.amount) {
        return a.amount > b.amount ? 1 : -1;
    }
    if (a.date != b.date) {
        return a.date > b.date ? 1 : -1;
    }
    
    if (a.location_id != b.location_id){
        return a.location_id > b.location_id ? 1 : -1;
    }
    
    return 0;
}


int cmp_by_payment_currency_amount_tax_items(const Transaction& a, const Transaction& b) {
    if (a.payment_method != b.payment_method){
        return a.payment_method > b.payment_method ? 1 : -1;
    }
    
    if (a.currency != b.currency){
        return a.currency > b.currency ? 1 : -1;
    }

    if (a.amount != b.amount) {
        return a.amount > b.amount ? 1 : -1;
    }
    
    if (a.tax_amount != b.tax_amount) {
        return a.tax_amount > b.tax_amount ? 1 : -1;
    }
    
    if (a.items_count != b.items_count) {
       return a.items_count > b.items_count ? 1 : -1;
    }
    
    return 0;
}

}
}