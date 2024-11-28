#include "../../include/Pair/Pair.hpp"

namespace tmn {

template <typename T1, typename T2>
Pair<T1, T2>::Pair() noexcept : first(T1()), second(T2()) {}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(const T1& first, const T2& second) noexcept: first(first), second(second) {}

template <typename T1, typename T2>
Pair<T1, T2>::Pair(const T1& first, T2&& second) noexcept : first(first), second(std::move(second)) {}

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

template <typename T1, typename T2>
tmn::Pair<T1, T2> make_pair(const T1& first, const T2& second) noexcept {
    return tmn::Pair<T1, T2>(first, second);
}

template <typename T1, typename T2>
tmn::Pair<const T1, T2> make_pair(const T1& first, T2&& second) noexcept {
    return tmn::Pair<const T1, T2>(first, std::move(second));
}

template <typename T1, typename T2>
tmn::Pair<T1, T2> make_pair(T1&& first, const T2& second) noexcept {
    return tmn::Pair<T1, T2>(std::move(first), second);
}

template <typename T1, typename T2>
tmn::Pair<T1, T2> make_pair(T1&& first, T2&& second) noexcept {
    return tmn::Pair<T1, T2>(std::move(first), std::move(second));
}

}