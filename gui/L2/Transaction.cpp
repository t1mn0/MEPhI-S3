#include <sstream>

#include "Transaction.hpp"

std::string to_string(const Transaction& t) {
    std::stringstream ss;
    ss << t.transaction_id << "," << t.user_id
    << "," << t.payment_method << "," << t.details << ","
    << t.date << "," << t.amount << "," << t.tax_amount << ","
    << t.discount << "," << t.items_count << "," << t.payment_status
    << "," << t.card_number << "," << t.location_id  << "," << t.is_refundable << "," << t.currency;
    return ss.str();
}


Transaction from_string(const std::string& s) {
    Transaction t;
    std::stringstream ss(s);
    std::string temp;

    std::getline(ss, t.transaction_id, ',');
    std::getline(ss, t.user_id, ',');
    std::getline(ss, t.payment_method, ',');
    std::getline(ss, t.details, ',');
    std::getline(ss, t.date, ',');
    std::getline(ss, temp, ',');
    t.amount = std::stod(temp);
    std::getline(ss, temp, ',');
    t.tax_amount = std::stod(temp);
    std::getline(ss, temp, ',');
    t.discount = std::stod(temp);
    std::getline(ss, temp, ',');
    t.items_count = std::stoi(temp);
    std::getline(ss, temp, ',');
    t.payment_status = std::stoi(temp);
    std::getline(ss, temp, ',');
    t.card_number = std::stoll(temp);
    std::getline(ss, temp, ',');
    t.location_id = std::stoi(temp);
    std::getline(ss, temp, ',');
    t.is_refundable = (temp == "1");
    std::getline(ss, t.currency, ',');
        
    return t;
}

bool Transaction::operator>(const Transaction& other) const {
    return transaction_id > other.transaction_id;
}

bool Transaction::operator<(const Transaction& other) const {
    return transaction_id < other.transaction_id;
}