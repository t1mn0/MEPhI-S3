#pragma once

#include <utility>

namespace tmn {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Conditional

template <bool B, typename U, typename V>
struct conditional;

template <typename U, typename V>
struct conditional<true, U, V>;

template <bool B, typename U, typename V>
using conditional_t = typename conditional<B, U, V>::type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Swap

template <typename T>
void swap(T& x, T& y);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Move & forward

template <typename T>
std::remove_reference_t<T>&& move(T&& x) noexcept;

template <typename T>
T&& forward(std::remove_reference_t<T>& x) noexcept;

// Function overload when forward accepts the rvalue-result of the function
template <typename T>
T&& forward(std::remove_reference_t<T>&& x) noexcept;


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// default_delete

template <typename T>
struct default_delete;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// any and idiom "type erasure"

class any;

template <typename T>
T& any_cast(any& a);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

}