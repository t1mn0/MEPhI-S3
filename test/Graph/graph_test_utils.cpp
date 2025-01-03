#include <cmath> // std::sqrt
#include <random>

#include "graph_test_utils.hpp"

bool is_symmetric_matrix(const Matrix& matrix) {
    if (matrix.empty() || matrix.size() != matrix[0].size()) {
        return false;
    }

    size_t n = matrix.size();
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            if (matrix[i][j] != matrix[j][i]) {
                return false;
            }
        }
    }
    return true;
}

complex_num::complex_num(double r, double i) : real(r), imag(i) {}

double complex_num::abs() const noexcept {
  return std::sqrt(real * real + imag * imag);
}

bool complex_num::operator<(const complex_num& other) const {
    return real * real + imag * imag < other.real * other.real + other.imag * other.imag;
}

bool complex_num::operator>(const complex_num& other) const { return other < *this; }
bool complex_num::operator<=(const complex_num& other) const { return !(*this > other); }
bool complex_num::operator>=(const complex_num& other) const { return !(*this < other); }
bool complex_num::operator==(const complex_num& other) const { return real == other.real && imag == other.imag; }
bool complex_num::operator!=(const complex_num& other) const { return !(*this == other); }

std::string roadstatus_to_string(RoadStatus status) {
    switch (status) {
        case RoadStatus::Normal:
            return "Normal";
        case RoadStatus::Congested:
            return "Congested";
        case RoadStatus::Closed:
            return "Closed";
        default:
            return "Unknown";
    }
}


const char* alph = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

std::string generate_small_string() {
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::uniform_int_distribution<> length_dist(3, 8);
    
    std::uniform_int_distribution<> char_dist(0, 61);
    
    int length = length_dist(gen);
    std::string result;
    
    for (int i = 0; i < length; ++i) {
        result += alph[char_dist(gen)];
    }
    
    return result;
}