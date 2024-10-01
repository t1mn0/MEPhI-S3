#pragma once

namespace tmn {

template <typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;

    Pair() noexcept;

    Pair(const T1& first, const T2& second) noexcept;

    Pair(const Pair<T1, T2>& other) noexcept;

    Pair(Pair<T1, T2>&& other) noexcept;

    Pair<T1, T2>& operator=(const Pair<T1, T2>& other) noexcept;

    bool operator==(const Pair<T1, T2>& other) noexcept;

    ~Pair() = default;
};

}

#include "../../src/Pair/Pair.tpp"