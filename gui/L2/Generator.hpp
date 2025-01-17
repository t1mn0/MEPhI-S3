#pragma once

#include <string>

#include "Transaction.hpp"

namespace tmn {

std::string generate_random_date();
Transaction generate_random_transaction();
void generate_transactions_to_file(long long target_size_bytes, std::string filepath);

}