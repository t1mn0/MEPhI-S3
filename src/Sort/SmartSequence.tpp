#include "../../include/Sort/SmartSequence.hpp"
#include "../../include/Exceptions/LogicException.hpp"

namespace tmn {
namespace sequence {

template <typename T>
SmartSequence<T>::SmartSequence() : _capacity(BASE_CAPACITY) {
    _array = smart_ptr::UniquePtr<T[]>(reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]));
}

template <typename T>
SmartSequence<T>::SmartSequence(std::size_t _size) : _size(_size), _capacity(_size * 2) {
    _array = smart_ptr::UniquePtr<T[]>(reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]));
}

template <typename T>
SmartSequence<T>::SmartSequence(std::size_t _size, const T& default_element) : _size(_size), _capacity(_size * 2) {
    T* raw_array = reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]);
    std::size_t i = 0;
    try{
        for (; i < _size; ++i){
            new (raw_array + i) T(default_element);
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            (raw_array + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(raw_array);
        throw;
    }
    _array = smart_ptr::UniquePtr<T[]>(raw_array);
}

template <typename T>
SmartSequence<T>::SmartSequence(const SmartSequence<T>& other) : _size(other._size), _capacity(other._capacity){
    T* raw_array = reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]);
    std::size_t i = 0;
    try{
        for (; i < _size; ++i){
            new (raw_array + i) T(other.get(i)); 
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            (raw_array + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(raw_array);
        throw;
    }
    _array = smart_ptr::UniquePtr<T[]>(raw_array);
}

template <typename T>
SmartSequence<T>::SmartSequence(SmartSequence<T>&& other) noexcept : 
    _array(std::move(other._array)), _size(other.size()), _capacity(other.capacity()) {
    
    other._size = 0;
    other._capacity = 0;
}

template <typename T>
SmartSequence<T>::SmartSequence(const std::initializer_list<T>& lst) : _size(lst.size()), _capacity(_size * 2){
    T* raw_array = reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]);
    std::size_t i = 0;
    try{
        for (auto element : lst){
            new (raw_array + i) T(element);
            ++i;
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            (raw_array + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(raw_array);
        throw;
    }
    _array = smart_ptr::UniquePtr<T[]>(raw_array);
}

template <typename T>
void SmartSequence<T>::swap(SmartSequence<T>& other){
    _array.swap(other._array);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
}

template <typename T>
SmartSequence<T>& SmartSequence<T>::operator=(const SmartSequence<T>& other){
    T* raw_array = reinterpret_cast<T*>(new int8_t[other._capacity * sizeof(T)]);
    std::size_t i = 0;
    try{
        for (; i < other._size; ++i){
            new (raw_array + i) T(*(other._array.get() + i)); 
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            (raw_array + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(raw_array);
        throw;
    }

    for (std::size_t i = 0; i < _size; ++i){
        (_array.get() + i)->~T();
    }

    _array = tmn::smart_ptr::UniquePtr<T[]>(raw_array);
    _size = other._size;
    _capacity = other._capacity;

    return *this;
}

template <typename T>
SmartSequence<T>::~SmartSequence(){
    T* ptr = _array.release();
    for (std::size_t i = 0; i < _size; ++i) {
        (ptr + i)->~T();
    }
    delete[] reinterpret_cast<int8_t*>(ptr);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Capacity & size :

template <typename T>
std::size_t SmartSequence<T>::size() const noexcept {
    return _size;
}

template <typename T>
std::size_t SmartSequence<T>::capacity() const noexcept {
    return _capacity;
}

template <typename T>
bool SmartSequence<T>::empty() const noexcept{
    return _size == 0;
}

template <typename T>
void SmartSequence<T>::reserve(std::size_t new_capacity){
    if (new_capacity <= _capacity){ return; }

    T* new_raw_array = reinterpret_cast<T*>(new int8_t[new_capacity * sizeof(T)]);

    std::size_t i = 0;
    try{
        for (; i < _size; ++i){
            new (new_raw_array + i) T((_array.get())[i]);
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            (new_raw_array + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(new_raw_array);
        throw;
    }

    for (std::size_t i = 0; i < _size; ++i){
        (_array.get() + i)->~T();
    }

    _array.~UniquePtr();
    _array = smart_ptr::UniquePtr<T[]>(new_raw_array);
    _capacity = new_capacity;
}

template <typename T>
void SmartSequence<T>::resize(std::size_t new_size, const T& value){
    if (new_size > _capacity){
        reserve(new_size * 2);
    }

    for(std::size_t i = _size; i < new_size; ++i){
        new (_array.get() + i) T(value);
    }

    _size = new_size;
}

template <typename T>
SmartSequence<T>& SmartSequence<T>::push_back(const T& value){
    if (_size == _capacity){
        reserve(_capacity > 0 ? 2 * _capacity : BASE_CAPACITY);
    }

    new (_array.get() + _size) T(value);
    ++_size;
    return *this;
}

template <typename T>
SmartSequence<T>& SmartSequence<T>::push_back(T&& value){
    if (_size == _capacity){
        reserve(_capacity > 0 ? 2 * _capacity : BASE_CAPACITY);
    }

    new (_array.get() + _size) T(std::move(value));
    ++_size;
    return *this;
}

template <typename T>
SmartSequence<T>& SmartSequence<T>::pop_back(){
    if (_size == 0) throw tmn::exception::LogicException("Attempt to delete from an empty dynamic array sequence");

    (_array.get() + (_size - 1))->~T();
    --_size;
    return *this;
}

template <typename T>
template <typename... Args>
void SmartSequence<T>::emplace_back(Args... args) {
    if (_size == _capacity){
        reserve(_capacity > 0 ? 2 * _capacity : BASE_CAPACITY);
    }

    new (_array.get() + _size) T(std::forward<Args>(args)...);
    ++_size;
}

template <typename T>
SmartSequence<T>& SmartSequence<T>::clear() {
    for (std::size_t i = 0; i < _size; ++i) {
        (_array.get() + i)->~T(); 
    }
    _size = 0;
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Element access methods :

template <typename T>
const T& SmartSequence<T>::front() const {
    return get(0);
}

template <typename T>
T& SmartSequence<T>::front() {
    return get(0);
}

template <typename T>
const T& SmartSequence<T>::back() const {
    return get(_size - 1);
}

template <typename T>
T& SmartSequence<T>::back() {
    return get(_size - 1);
}

template <typename T>
SmartSequence<T>& SmartSequence<T>::set(std::size_t index, const T& item) {
    if (index >= _size) {
        throw tmn::exception::LogicException("Accessing an element by out of range index in dynamic array sequence");
    }
    (_array.get())[index] = item;
    return *this;
}

template <typename T>
SmartSequence<T>& SmartSequence<T>::set(std::size_t index, T&& item) {
    if (index >= _size) {
        throw tmn::exception::LogicException("Accessing an element by out of range index in dynamic array sequence");
    }
    (_array.get())[index] = std::move(item);
    return *this;
}

template <typename T>
T& SmartSequence<T>::get(std::size_t index) {
    if (index >= _size) {
        throw tmn::exception::LogicException("Accessing an element by out of range index in dynamic array sequence");
    }

    return (_array.get())[index];
}

template <typename T>
const T& SmartSequence<T>::get(std::size_t index) const {
    if (index >= _size) {
        throw tmn::exception::LogicException("Accessing an element by out of range index in dynamic array sequence");
    }
    
    return (_array.get())[index];
}

template <typename T>
T& SmartSequence<T>::operator[](std::size_t index) noexcept{
    return (_array.get())[index];
}

template <typename T>
const T& SmartSequence<T>::operator[](std::size_t index) const noexcept{
    return (_array.get())[index];
}

}
}