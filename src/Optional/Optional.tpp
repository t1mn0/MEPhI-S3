#include "../../include/Optional/Optional.hpp"
#include "../../include/Exceptions/Exception.hpp"



namespace tmn {



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion & destructors :

template <typename T>
Optional<T>::Optional(const T& new_value) : initialized(true) {
    new (_value) T(new_value);
}

template <typename T>
Optional<T>::Optional() : initialized(false) {}

template <typename T>
void Optional<T>::swap(Optional<T>& other){
    std::swap(_value, other._value);
    std::swap(initialized, other.initialized);
}

template <typename T>
Optional<T>& Optional<T>::operator=(const Optional& other) noexcept {
    Optional tmp(other);
    swap(tmp);
    return *this;
}

template <typename T>
Optional<T>::operator bool() const noexcept {
    return initialized;
}

template <typename T>
Optional<T>::~Optional(){
    if (initialized){
        reinterpret_cast<T*>(_value)->~T();
    }
}




// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Observers :

template <typename T>
bool Optional<T>::has_value() const noexcept {
    return initialized;
}

template <typename T>
T& Optional<T>::operator*() & {
    return reinterpret_cast<T&>(*_value);
}

template <typename T>
const T& Optional<T>::operator*() const & {
    return reinterpret_cast<const T&>(*_value);
}

template <typename T>
T&& Optional<T>::operator*() && {
    return reinterpret_cast<T&&>(*_value);
}

template <typename T>
const T&& Optional<T>::operator*() const && {
    return reinterpret_cast<const T&&>(*_value);
}

template <typename T>
T& Optional<T>::value() & {
    if (!initialized) throw tmn_exception::Exception("Bad optional access");
    return reinterpret_cast<T&>(*_value);
}

template <typename T>
const T& Optional<T>::value() const & {
    if (!initialized) throw tmn_exception::Exception("Bad optional access");
    return reinterpret_cast<const T&>(*_value);
}

template <typename T>
T&& Optional<T>::value() && {
    if (!initialized) throw tmn_exception::Exception("Bad optional access");
    return reinterpret_cast<T&&>(*_value);
}

template <typename T>
const T&& Optional<T>::value() const && {
    if (!initialized) throw tmn_exception::Exception("Bad optional access");
    return reinterpret_cast<const T&&>(*_value);
}

// Since C++23
// decltype(auto) value(this auto&& self) {
//     if constexpr (!initialized) throw tmn_exception::Exception("Bad optional access");
//     return reinterpret_cast<decltype(std::forward_like<decltype(self)>(std::declval<T>()))>(self._value);
// }

template <typename T>
const T& Optional<T>::value_or(const T& other) const noexcept {
    return initialized ? reinterpret_cast<T&>(*_value) : other;
}

}