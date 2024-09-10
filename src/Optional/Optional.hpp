#pragma once



namespace tmn {



template <typename T>
class Optional {
private:
    char _value[sizeof(T)];
    bool initialized;

public:
// Constructors & assignment & conversion & destructors :
    Optional(const T& new_value);
    Optional();

    void swap(Optional<T>& other);
    Optional& operator=(const Optional& other) noexcept;

    operator bool() const noexcept;

    ~Optional();

// Observers :
    bool has_value() const noexcept;

    T& operator*() &;
    const T& operator*() const &;
    T&& operator*() &&;
    const T&& operator*() const &&;

    T& value() &;
    const T& value() const &;
    T&& value() &&;
    const T&& value() const &&;

    const T& value_or(const T& other) const noexcept;
};



}



#include "Optional.cpp"