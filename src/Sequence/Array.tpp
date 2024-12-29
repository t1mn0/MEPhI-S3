#include "../../include/Sequence/Array.hpp"
#include "../../include/Exceptions/LogicException.hpp"

namespace tmn {
namespace sequence {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator implementation :

template <typename T, std::size_t N>
template <bool isConst>
Array<T, N>::common_iterator<isConst>& 
Array<T, N>::common_iterator<isConst>::operator=(const common_iterator<isConst>& other){
    ptr = other.ptr;
    return *this;
}

template <typename T, std::size_t N>
template <bool isConst>
typename Array<T, N>::common_iterator<isConst>::conditional_ref
Array<T, N>::common_iterator<isConst>::operator*() const {
    return *ptr;
}

template <typename T, std::size_t N>
template <bool isConst>
typename Array<T, N>::common_iterator<isConst>::conditional_ptr
Array<T, N>::common_iterator<isConst>::operator->() const {
    return ptr;
}

template <typename T, std::size_t N>
template <bool isConst>
bool Array<T, N>::common_iterator<isConst>::operator==(const common_iterator<isConst>& other) const {
    return ptr == other.ptr;
}

template <typename T, std::size_t N>
template <bool isConst>
bool Array<T, N>::common_iterator<isConst>::operator!=(const common_iterator<isConst>& other) const {
    return ptr != other.ptr;
}

template <typename T, std::size_t N>
template <bool isConst>
Array<T, N>::common_iterator<isConst>& 
Array<T, N>::common_iterator<isConst>::operator++() {
    ++ptr;
    return *this;
}

template <typename T, std::size_t N>
template <bool isConst>
Array<T, N>::common_iterator<isConst> 
Array<T, N>::common_iterator<isConst>::operator++(int) {
   common_iterator<isConst> tmp(*this);
   ++ptr;
   return tmp;
}

template <typename T, std::size_t N>
template <bool isConst>
Array<T, N>::common_iterator<isConst>& 
Array<T, N>::common_iterator<isConst>::operator--() {
    --ptr;
    return *this;
}

template <typename T, std::size_t N>
template <bool isConst>
Array<T, N>::common_iterator<isConst> 
Array<T, N>::common_iterator<isConst>::operator--(int) {
   common_iterator<isConst> tmp(*this);
   --ptr;
   return tmp;
}

template <typename T, std::size_t N>
template <bool isConst>
Array<T, N>::common_iterator<isConst> 
Array<T, N>::common_iterator<isConst>::operator+(Array<T, N>::common_iterator<isConst>::difference_type n) const {
   return common_iterator<isConst>(ptr + n);
}

template <typename T, std::size_t N>
template <bool isConst>
Array<T, N>::common_iterator<isConst> 
Array<T, N>::common_iterator<isConst>::operator-(Array<T, N>::common_iterator<isConst>::difference_type n) const {
   return common_iterator<isConst>(ptr - n);
}

template <typename T, std::size_t N>
template <bool isConst>
typename Array<T, N>::common_iterator<isConst>::difference_type
Array<T, N>::common_iterator<isConst>::operator-(const Array<T, N>::common_iterator<isConst>& other) const {
   return ptr - other.ptr;
}

template <typename T, std::size_t N>
template <bool isConst>
Array<T, N>::common_iterator<isConst>& 
Array<T, N>::common_iterator<isConst>::operator+=(Array<T, N>::common_iterator<isConst>::difference_type n) {
    ptr += n;
    return *this;
}

template <typename T, std::size_t N>
template <bool isConst>
Array<T, N>::common_iterator<isConst>& 
Array<T, N>::common_iterator<isConst>::operator-=(Array<T, N>::common_iterator<isConst>::difference_type n) {
    ptr -= n;
    return *this;
}

template <typename T, std::size_t N>
template <bool isConst>
bool Array<T, N>::common_iterator<isConst>::operator<(const common_iterator<isConst>& other) const {
    return ptr < other.ptr;
}

template <typename T, std::size_t N>
template <bool isConst>
bool Array<T, N>::common_iterator<isConst>::operator>(const common_iterator<isConst>& other) const {
    return ptr > other.ptr;
}

template <typename T, std::size_t N>
template <bool isConst>
bool Array<T, N>::common_iterator<isConst>::operator<=(const common_iterator<isConst>& other) const {
    return ptr <= other.ptr;
}

template <typename T, std::size_t N>
template <bool isConst>
bool Array<T, N>::common_iterator<isConst>::operator>=(const common_iterator<isConst>& other) const {
    return ptr > other.ptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion :

template <typename T, std::size_t N>
Array<T, N>::Array() {
    array = reinterpret_cast<T*>(new int8_t[N * sizeof(T)]);
    std::size_t i = 0;
    try{
        for (; i < N; ++i){
            new (array + i) T(); 
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            (array + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(array);
        throw;
    }
}

template <typename T, std::size_t N>
Array<T, N>::Array(const T& default_element) {
    array = reinterpret_cast<T*>(new int8_t[N * sizeof(T)]);
    std::size_t i = 0;
    try{
        for (; i < N; ++i){
            new (array + i) T(default_element); 
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            (array + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(array);
        throw;
    }
}

template <typename T, std::size_t N>
Array<T, N>::Array(const Array<T, N>& other) {
    array = reinterpret_cast<T*>(new int8_t[N * sizeof(T)]);
    std::size_t i = 0;
    try{
        for (; i < N; ++i){
            new (array + i) T(other.get(i)); 
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            (array + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(array);
        throw;
    }
}

template <typename T, std::size_t N>
Array<T, N>::Array(std::initializer_list<T> lst) {
    array = reinterpret_cast<T*>(new int8_t[N * sizeof(T)]);
    std::size_t i = 0;
    try{
        for (auto element : lst){
            new (array + i) T(element); 
            ++i;
            if (i == N) break;
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            (array + j)->~T();
        }
        delete[] reinterpret_cast<int8_t*>(array);
        throw;
    }
}

template <typename T, std::size_t N>
void Array<T, N>::swap(Array<T, N>& other){
    std::swap(array, other.array);
}

template <typename T, std::size_t N>
Array<T, N>& Array<T, N>::operator=(const Array<T, N>& other){
    Array<T, N> new_array(other);
    swap(new_array);
    return *this;
}

template <typename T, std::size_t N>
Array<T, N>::~Array(){
    for (std::size_t i = 0; i < N; ++i) {
        (array + i)->~T();
    }

    if (N != 0) {
        delete[] reinterpret_cast<int8_t*>(array);
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Capacity & size :

template <typename T, std::size_t N>
std::size_t Array<T, N>::size() const noexcept {
    return N;
}

template <typename T, std::size_t N>
bool Array<T, N>::empty() const noexcept{
    return N == 0;
}

template <typename T, std::size_t N>
void Array<T, N>::fill(const T& value){
    for (std::size_t i = 0; i < N; ++i){
        array[i] = value;
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Element access methods :


template <typename T, std::size_t N>
const T& Array<T, N>::front() const {
    return get(0);
}

template <typename T, std::size_t N>
T& Array<T, N>::front() {
    return get(0);
}

template <typename T, std::size_t N>
const T& Array<T, N>::back() const {
    return get(N - 1);
}

template <typename T, std::size_t N>
T& Array<T, N>::back() {
    return get(N - 1);
}

template <typename T, std::size_t N>
Array<T, N>& Array<T, N>::set(std::size_t index, const T& item) {
    if (index >= N) {
        throw tmn::exception::LogicException("Accessing an element by out of range index in array sequence");
    }
    array[index] = item;
    return *this;
}

template <typename T, std::size_t N>
T& Array<T, N>::get(std::size_t index) {
    if (index >= N) {
        throw tmn::exception::LogicException("Accessing an element by out of range index in array sequence");
    }

    return array[index];
}

template <typename T, std::size_t N>
const T& Array<T, N>::get(std::size_t index) const {
    if (index >= N) {
        throw tmn::exception::LogicException("Accessing an element by out of range index in array sequence");
    }
    
    return array[index];
}

template <typename T, std::size_t N>
T& Array<T, N>::operator[](std::size_t index) noexcept{
    return array[index];
}

template <typename T, std::size_t N>
const T& Array<T, N>::operator[](std::size_t index) const noexcept{
    return array[index];
}

template <typename T, std::size_t N>
const T* Array<T, N>::data() const noexcept{
    return array;
}

template <typename T, std::size_t N>
T* Array<T, N>::data() noexcept{
    return array;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator methods :

template <typename T, std::size_t N>
typename Array<T, N>::iterator Array<T, N>::begin() noexcept {
    return iterator(array);
}

template <typename T, std::size_t N>
typename Array<T, N>::const_iterator Array<T, N>::begin() const noexcept {
    return const_iterator(array);
}

template <typename T, std::size_t N>
typename Array<T, N>::const_iterator Array<T, N>::cbegin() const noexcept {
    return const_iterator(array);
}

template <typename T, std::size_t N>
typename Array<T, N>::iterator Array<T, N>::end() noexcept {
    return iterator(array + N);
}

template <typename T, std::size_t N>
typename Array<T, N>::const_iterator Array<T, N>::end() const noexcept {
    return const_iterator(array + N);
}

template <typename T, std::size_t N>
typename Array<T, N>::const_iterator Array<T, N>::cend() const noexcept {
    return const_iterator(array + N);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

}
}