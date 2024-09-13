#include "../../include/Pair/Pair.hpp"

namespace tmn {

template <typename T1, typename T2>
Pair<T1, T2>::Pair() : first(T1()), second(T2()) {}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(const T1& first, const T2& second) : first(first), second(second) {}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(const Pair<T1, T2>& other) : first(other.first), second(other.second) {}

template <typename T1, typename T2>
Pair<T1, T2>& Pair<T1, T2>::operator=(const Pair<T1, T2>& other) {
    first = other.first;
    second = other.second;
    return *this;
}

template <typename T1, typename T2>
bool Pair<T1, T2>::operator==(const Pair<T1, T2>& other) {
    return this->first == other.first && this->second == other.second;
}

}