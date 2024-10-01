#include "../../include/Pair/Pair.hpp"

namespace tmn {

template <typename T1, typename T2>
Pair<T1, T2>::Pair() noexcept : first(T1()), second(T2()) {}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(const T1& first, const T2& second) noexcept: first(first), second(second) {}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(const Pair<T1, T2>& other) noexcept : first(other.first), second(other.second) {}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(Pair<T1, T2>&& other) noexcept : first(std::move(first)), second(std::move(other.second)) {}

template <typename T1, typename T2>
Pair<T1, T2>& Pair<T1, T2>::operator=(const Pair<T1, T2>& other) noexcept {
    first = other.first;
    second = other.second;
    return *this;
}

template <typename T1, typename T2>
bool Pair<T1, T2>::operator==(const Pair<T1, T2>& other) noexcept {
    return this->first == other.first && this->second == other.second;
}

}