#pragma once

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Pair

namespace tmn {

template <typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;

    Pair() : first(T1()), second(T2()) {}

    Pair(const T1& first, const T2& second) : first(first), second(second) {}

    Pair(const Pair<T1, T2>& other) : first(other.first), second(other.second) {}

    Pair<T1, T2>& operator=(const Pair<T1, T2>& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    bool operator==(const Pair<T1, T2>& other) {
        return this->first == other.first && this->second == other.second;
    }

    ~Pair() {}
};

}