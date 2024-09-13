#pragma once



namespace tmn {

template <typename T1, typename T2>
class Pair {
public:
    T1 first;
    T2 second;

    Pair();

    Pair(const T1& first, const T2& second);

    Pair(const Pair<T1, T2>& other) : first(other.first), second(other.second);

    Pair<T1, T2>& operator=(const Pair<T1, T2>& other);

    bool operator==(const Pair<T1, T2>& other);

    ~Pair() = default;
};

}