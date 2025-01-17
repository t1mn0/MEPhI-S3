#include <random>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "Transaction.hpp"

namespace tmn {
namespace sequence{

std::string generate_random_date() {
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> day_distribution(1, 31);
    std::uniform_int_distribution<int> month_distribution(1, 12);
    std::uniform_int_distribution<int> year_distribution(2020, 2024);

    int day = day_distribution(generator);
    int month = month_distribution(generator);
    int year = year_distribution(generator);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << day << "-"
       << std::setfill('0') << std::setw(2) << month << "-" << year;

    return ss.str();
}

Transaction generate_random_transaction() {
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> int_distribution(0, 1000);
    std::uniform_real_distribution<double> double_distribution(0.0, 1000.0);
    std::uniform_real_distribution<double> discount_distribution(0.0, 100.0);
    std::uniform_int_distribution<long long> long_distribution(1000000000000000, 9999999999999999);
    std::uniform_int_distribution<int> bool_distribution(0, 1);
    std::uniform_int_distribution<int> status_distribution(0, 2);

    std::vector<std::string> payment_methods = { "Credit Card", "Debit Card", "PayPal", "Bank Transfer", "MIR pay", "Google pay", "Apple pay" };
    std::vector<std::string> currencies = { "USD", "EUR", "GBP", "JPY", "CAD", "AUD", "CHF","CNY"};
    std::uniform_int_distribution<int> payment_distribution(0, payment_methods.size() - 1);
    std::uniform_int_distribution<int> currencie_distribution(0, currencies.size() - 1);

    Transaction t;
    t.transaction_id = "TXN-" + std::to_string(int_distribution(generator));
    t.user_id = "USER-" + std::to_string(int_distribution(generator));
    t.payment_method = payment_methods[payment_distribution(generator)];
    t.details = "Random transaction details";
    t.date = generate_random_date();
    t.amount = double_distribution(generator);
    t.tax_amount = double_distribution(generator) /10;
    t.discount = discount_distribution(generator)/10;
    t.items_count = int_distribution(generator) % 10 + 1;
    t.payment_status = status_distribution(generator);
    t.card_number = long_distribution(generator);
    t.location_id = int_distribution(generator);
    t.is_refundable = bool_distribution(generator);
    t.currency = currencies[currencie_distribution(generator)];
    return t;
}

void generate_transactions_to_file(long long target_size_bytes, std::string filepath = std::string(EXECUTABLE_PATH) + "/Transactions/transactions.txt") {
    std::ofstream file(filepath);
    if (!file.is_open()) {
    std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }

    long long current_size = 0;
    while (current_size < target_size_bytes) {
        Transaction t = generate_random_transaction();
        std::string s = to_string(t);
        file << s << "\n";
        current_size += s.size() + 1;
    }

    file.close();
}

}
}
