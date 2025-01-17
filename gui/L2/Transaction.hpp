#pragma once

#include <string>
#include <iostream>

struct Transaction {
    std::string transaction_id;
    std::string user_id;
    std::string payment_method;
    std::string details;
    std::string date;
    double amount;
    double tax_amount;
    double discount;
    int items_count;
    int payment_status;
    long long card_number;
    int location_id;
    bool is_refundable;
    std::string currency;

    bool operator>(const Transaction& other) const;
    bool operator<(const Transaction& other) const;
};

std::string to_string(const Transaction& t);
Transaction from_string(const std::string& s);