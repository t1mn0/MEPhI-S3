#include <sstream>
#include <iomanip>
#include <ctime>

#include "Transaction.hpp"

namespace tmn {
namespace sort {

int cmp_dates(const std::string& date1, const std::string& date2) {
    std::tm t1, t2;
    std::istringstream iss1(date1);
    std::istringstream iss2(date2);

    iss1 >> std::get_time(&t1, "%d-%m-%Y");
    iss2 >> std::get_time(&t2, "%d-%m-%Y");

    if (iss1.fail() || iss2.fail()) {
        return 0;
    }

    std::time_t time1 = mktime(&t1);
    std::time_t time2 = mktime(&t2);

    if (time1 < time2){
        return -1;
    }
    
    if (time1 > time2) {
        return 1;
    }
    
    return 0;
}

int cmp_by_dates(const Transaction& a, const Transaction& b) {
    return cmp_dates(a.date, b.date);
}

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