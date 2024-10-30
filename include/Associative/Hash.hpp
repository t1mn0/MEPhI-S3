#pragma once

#include <cstdint>
#include <cstring>
#include <functional>

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// h: T (fixed set) -> uint32_t (0 - 4 294 967 295)

namespace tmn_hash {

template <typename T>
unsigned int Hash(const T& value) {
  return std::hash<T>{}(value);
}

template<>
unsigned int inline Hash<char>(const char& c) {
    return static_cast<unsigned int>(c);
}

template<>
unsigned int inline Hash<short>(const short& value) {
    return static_cast<unsigned int>(value);
}

template<>
unsigned int inline Hash<int>(const int& value) {
    uint32_t temp = static_cast<uint32_t>(value);
    temp = (temp ^ (temp >> 16));
    return static_cast<unsigned int>(temp);
}

template<>
unsigned int inline Hash<unsigned int>(const unsigned int &value) {
    unsigned int temp = value;
    temp = (temp ^ (temp >> 16));
    return static_cast<unsigned int>(temp);
}

template<>
unsigned int inline Hash<long int>(const long int &value) {
    uint64_t temp = static_cast<uint64_t>(value);
    temp = (temp ^ (temp >> 32));
    temp = (temp ^ (temp >> 16));
    return static_cast<unsigned int>(temp);
}

template<>
unsigned int inline Hash<unsigned long int>(const unsigned long int &value) {
    unsigned long int temp = value;
    temp = (temp ^ (temp >> 32));
    temp = (temp ^ (temp >> 16));
    return static_cast<unsigned int>(value);
}

template<>
unsigned int inline Hash<float>(const float& value) {
    uint32_t temp;
    std::memcpy(&temp, &value, sizeof(float));
    temp = (temp ^ (temp >> 16));
    return static_cast<unsigned int>(temp);
}

template<>
unsigned int inline Hash<double>(const double& value) {
    uint64_t temp;
    std::memcpy(&temp, &value, sizeof(double));
    temp = (temp ^ (temp >> 32));
    temp = (temp ^ (temp >> 16));
    return static_cast<unsigned int>(temp);
}

}