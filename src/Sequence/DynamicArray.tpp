#include "../../include/Sequence/DynamicArray.hpp"
#include "../../include/Exceptions/LogicException.hpp"

namespace tmn_sequence{

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator implementation :

template <typename T, class Allocator>
template <bool isConst>
DynamicArray<T, Allocator>::common_iterator<isConst>& 
DynamicArray<T, Allocator>::common_iterator<isConst>::operator=(const common_iterator<isConst>& other){
    ptr = other.ptr;
    return *this;
}

template <typename T, class Allocator>
template <bool isConst>
typename DynamicArray<T, Allocator>::common_iterator<isConst>::conditional_ref
DynamicArray<T, Allocator>::common_iterator<isConst>::operator*() const {
    return *ptr;
}

template <typename T, class Allocator>
template <bool isConst>
typename DynamicArray<T, Allocator>::common_iterator<isConst>::conditional_ptr
DynamicArray<T, Allocator>::common_iterator<isConst>::operator->() const {
    return ptr;
}

template <typename T, class Allocator>
template <bool isConst>
bool DynamicArray<T, Allocator>::common_iterator<isConst>::operator==(const common_iterator<isConst>& other) const {
    return ptr == other.ptr;
}

template <typename T, class Allocator>
template <bool isConst>
bool DynamicArray<T, Allocator>::common_iterator<isConst>::operator!=(const common_iterator<isConst>& other) const {
    return ptr != other.ptr;
}

template <typename T, class Allocator>
template <bool isConst>
DynamicArray<T, Allocator>::common_iterator<isConst>& 
DynamicArray<T, Allocator>::common_iterator<isConst>::operator++() {
    ++ptr;
    return *this;
}

template <typename T, class Allocator>
template <bool isConst>
DynamicArray<T, Allocator>::common_iterator<isConst> 
DynamicArray<T, Allocator>::common_iterator<isConst>::operator++(int) {
   common_iterator<isConst> tmp(*this);
   ++ptr;
   return tmp;
}

template <typename T, class Allocator>
template <bool isConst>
DynamicArray<T, Allocator>::common_iterator<isConst>& 
DynamicArray<T, Allocator>::common_iterator<isConst>::operator--() {
    --ptr;
    return *this;
}

template <typename T, class Allocator>
template <bool isConst>
DynamicArray<T, Allocator>::common_iterator<isConst> 
DynamicArray<T, Allocator>::common_iterator<isConst>::operator--(int) {
   common_iterator<isConst> tmp(*this);
   --ptr;
   return tmp;
}

template <typename T, class Allocator>
template <bool isConst>
DynamicArray<T, Allocator>::common_iterator<isConst> 
DynamicArray<T, Allocator>::common_iterator<isConst>::operator+(DynamicArray<T, Allocator>::common_iterator<isConst>::difference_type n) const {
   return common_iterator<isConst>(ptr + n);
}

template <typename T, class Allocator>
template <bool isConst>
DynamicArray<T, Allocator>::common_iterator<isConst> 
DynamicArray<T, Allocator>::common_iterator<isConst>::operator-(DynamicArray<T, Allocator>::common_iterator<isConst>::difference_type n) const {
   return common_iterator<isConst>(ptr - n);
}

template <typename T, class Allocator>
template <bool isConst>
typename DynamicArray<T, Allocator>::common_iterator<isConst>::difference_type
DynamicArray<T, Allocator>::common_iterator<isConst>::operator-(const DynamicArray<T, Allocator>::common_iterator<isConst>& other) const {
   return ptr - other.ptr;
}

template <typename T, class Allocator>
template <bool isConst>
DynamicArray<T, Allocator>::common_iterator<isConst>& 
DynamicArray<T, Allocator>::common_iterator<isConst>::operator+=(DynamicArray<T, Allocator>::common_iterator<isConst>::difference_type n) {
    ptr += n;
    return *this;
}

template <typename T, class Allocator>
template <bool isConst>
DynamicArray<T, Allocator>::common_iterator<isConst>& 
DynamicArray<T, Allocator>::common_iterator<isConst>::operator-=(DynamicArray<T, Allocator>::common_iterator<isConst>::difference_type n) {
    ptr -= n;
    return *this;
}

template <typename T, class Allocator>
template <bool isConst>
bool DynamicArray<T, Allocator>::common_iterator<isConst>::operator<(const common_iterator<isConst>& other) const {
    return ptr < other.ptr;
}

template <typename T, class Allocator>
template <bool isConst>
bool DynamicArray<T, Allocator>::common_iterator<isConst>::operator>(const common_iterator<isConst>& other) const {
    return ptr > other.ptr;
}

template <typename T, class Allocator>
template <bool isConst>
bool DynamicArray<T, Allocator>::common_iterator<isConst>::operator<=(const common_iterator<isConst>& other) const {
    return ptr <= other.ptr;
}

template <typename T, class Allocator>
template <bool isConst>
bool DynamicArray<T, Allocator>::common_iterator<isConst>::operator>=(const common_iterator<isConst>& other) const {
    return ptr > other.ptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion :

template <typename T, class Allocator>
DynamicArray<T, Allocator>::DynamicArray() : _capacity(4) {
    //array = reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]);
    array = allocator_traits::allocate(_alloc, _capacity);
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>::DynamicArray(std::size_t _size) : _size(_size), _capacity(_size * 2) {
    //array = reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]);
    array = allocator_traits::allocate(_alloc, _capacity);
    std::size_t i = 0;
    try{
        for (; i < _size; ++i){
            // new (array + i) T(); 
            allocator_traits::construct(_alloc, array + i);
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            // (array + j)->~T();
            allocator_traits::destroy(_alloc, array + j);
        }
        //delete[] reinterpret_cast<int8_t*>(array);
        allocator_traits::deallocate(_alloc, array, _capacity);
        throw;
    }
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>::DynamicArray(std::size_t _size, const T& default_element) : _size(_size), _capacity(_size * 2) {
    //array = reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]);
    array = allocator_traits::allocate(_alloc, _capacity);
    std::size_t i = 0;
    try{
        for (; i < _size; ++i){
            // new (array + i) T(default_element);
            allocator_traits::construct(_alloc, array + i, default_element);
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            // (array + j)->~T();
            allocator_traits::destroy(_alloc, array + j);
        }
        // delete[] reinterpret_cast<int8_t*>(array);
        allocator_traits::deallocate(_alloc, array, _capacity);
        throw;
    }
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>::DynamicArray(const DynamicArray<T, Allocator>& other) : _size(other._size), _capacity(other._capacity){
    // array = reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]);
    array = allocator_traits::allocate(_alloc, _capacity);
    std::size_t i = 0;
    try{
        for (; i < _size; ++i){
            // new (array + i) T(other.get(i)); 
            allocator_traits::construct(_alloc, array + i, other[i]);
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            // (array + j)->~T();
            allocator_traits::destroy(_alloc, array + j);
        }
        // delete[] reinterpret_cast<int8_t*>(array);
        allocator_traits::deallocate(_alloc, array, _capacity);
        throw;
    }
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>::DynamicArray(DynamicArray<T, Allocator>&& other) noexcept : 
    array(other.array), _size(other.size()), _capacity(other.capacity()), _alloc(std::move(other._alloc)) {
        
    other.array = nullptr; 
    other._size = 0;
    other._capacity = 0;
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>::DynamicArray(std::initializer_list<T> lst) : _size(lst.size()), _capacity(_size * 2){
    // array = reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]);
    array = allocator_traits::allocate(_alloc, _capacity);
    std::size_t i = 0;
    try{
        for (auto element : lst){
            // new (array + i) T(element); 
            allocator_traits::construct(_alloc, array + i, element);
            ++i;
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            // (array + j)->~T();
            allocator_traits::destroy(_alloc, array + j);
        }
        // delete[] reinterpret_cast<int8_t*>(array);
        allocator_traits::deallocate(_alloc, array, _capacity);
        throw;
    }
}

template <typename T, class Allocator>
void DynamicArray<T, Allocator>::swap(DynamicArray<T, Allocator>& other){
    std::swap(array, other.array);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>& DynamicArray<T, Allocator>::operator=(const DynamicArray<T, Allocator>& other){
    Allocator new_alloc = allocator_traits::propagate_on_container_copy_assignment::value ? other._alloc : _alloc;

    // T* new_array = reinterpret_cast<T*>(new int8_t[_capacity * sizeof(T)]);
    T* new_array = allocator_traits::allocate(new_alloc, other._capacity);
    std::size_t i = 0;
    try{
        for (; i < _size; ++i){
            // new (new_array + i) T(other.get(i)); 
            allocator_traits::construct(new_alloc, new_array + i, other[i]);
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            // (new_array + j)->~T();
            allocator_traits::destroy(new_alloc, new_array + j);
        }
        // delete[] reinterpret_cast<int8_t*>(array);
        allocator_traits::deallocate(new_alloc, new_array, other._capacity);
        throw;
    }

    for (std::size_t i = 0; i < _size; ++i){
        allocator_traits::destroy(_alloc, array + i);
    }

    array = new_array;
    _size = other._size;
    _capacity = other._capacity;
    _alloc = new_alloc;

    return *this;
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>::~DynamicArray(){
    for (std::size_t i = 0; i < _size; ++i) {
        // (array + i)->~T();
        allocator_traits::destroy(_alloc, array + i);
    }

    // delete[] reinterpret_cast<int8_t*>(array);
    allocator_traits::deallocate(_alloc, array, _capacity);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Capacity & size :

template <typename T, class Allocator>
std::size_t DynamicArray<T, Allocator>::size() const noexcept {
    return _size;
}

template <typename T, class Allocator>
std::size_t DynamicArray<T, Allocator>::capacity() const noexcept {
    return _capacity;
}

template <typename T, class Allocator>
bool DynamicArray<T, Allocator>::empty() const noexcept{
    return _size == 0;
}

template <typename T, class Allocator>
void DynamicArray<T, Allocator>::reserve(std::size_t new_capacity){
    if (new_capacity <= _capacity){ return; }

    // T* new_array = reinterpret_cast<T*>(new int8_t[new_capacity * sizeof(T)]); 
    T* new_array = allocator_traits::allocate(_alloc, new_capacity);

    std::size_t i = 0;
    try{
        for (; i < _size; ++i){
            // new (new_array + i) T(array[i]);
            allocator_traits::construct(_alloc, new_array + i, std::move_if_noexcept(array[i]));
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            //(new_array + j)->~T();
            allocator_traits::destroy(_alloc, new_array + j);
        }
        //delete[] reinterpret_cast<int8_t*>(new_array);
        allocator_traits::deallocate(_alloc, new_array, new_capacity);
        throw;
    }

    for (std::size_t i = 0; i < _size; ++i){
        // (array + i)->~T();
        allocator_traits::destroy(_alloc, array + i);
    }

    // delete[] reinterpret_cast<int8_t*>(array);
    allocator_traits::deallocate(_alloc, array, _capacity);
    array = new_array;
    _capacity = new_capacity;
}

template <typename T, class Allocator>
void DynamicArray<T, Allocator>::resize(std::size_t new_size, const T& value){
    if (new_size > _capacity){
        reserve(new_size);
    }

    for(std::size_t i = _size; i < new_size; ++i){
        // new (array + i) T(value);
        allocator_traits::construct(_alloc, array + i, value);
    }

    _size = new_size;
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>& DynamicArray<T, Allocator>::push_back(const T& value){
    if (_size == _capacity){
        reserve(_capacity > 0 ? 2 * _capacity : 4);
    }

    // new (array + _size) T(value);
    allocator_traits::construct(_alloc, array + _size, value);
    ++_size;
    return *this;
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>& DynamicArray<T, Allocator>::push_back(T&& value){
    if (_size == _capacity){
        reserve(_capacity > 0 ? 2 * _capacity : 4);
    }

    allocator_traits::construct(_alloc, array + _size, std::move(value));
    ++_size;
    return *this;
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>& DynamicArray<T, Allocator>::pop_back(){
    if (_size == 0) throw tmn_exception::LogicException("Attempt to delete from an empty dynamic array sequence");

    // (array + _size - 1)->~T();
    allocator_traits::destroy(_alloc, array + _size - 1);
    --_size;
    return *this;
}

template <typename T, class Allocator>
template <typename... Args>
void DynamicArray<T, Allocator>::emplace_back(Args... args) {
    if (_size == _capacity){
        reserve(_capacity > 0 ? 2 * _capacity : 4);
    }

    allocator_traits::construct(_alloc, array + _size, std::forward<Args>(args)...);
    ++_size;
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>& DynamicArray<T, Allocator>::clear() {
    for (std::size_t i = 0; i < _size; ++i) {
        // (array + i)->~T(); 
        allocator_traits::destroy(_alloc, array + i);
    }
    _size = 0;
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Element access methods :


template <typename T, class Allocator>
const T& DynamicArray<T, Allocator>::front() const {
    return get(0);
}

template <typename T, class Allocator>
T& DynamicArray<T, Allocator>::front() {
    return get(0);
}

template <typename T, class Allocator>
const T& DynamicArray<T, Allocator>::back() const {
    return get(_size - 1);
}

template <typename T, class Allocator>
T& DynamicArray<T, Allocator>::back() {
    return get(_size - 1);
}

template <typename T, class Allocator>
DynamicArray<T, Allocator>& DynamicArray<T, Allocator>::set(std::size_t index, const T& item) {
    if (index >= _size) {
        throw tmn_exception::LogicException("Accessing an element by out of range index in dynamic array sequence");
    }
    array[index] = item;
    return *this;
}

template <typename T, class Allocator>
T& DynamicArray<T, Allocator>::get(std::size_t index) {
    if (index >= _size) {
        throw tmn_exception::LogicException("Accessing an element by out of range index in dynamic array sequence");
    }

    return array[index];
}

template <typename T, class Allocator>
const T& DynamicArray<T, Allocator>::get(std::size_t index) const {
    if (index >= _size) {
        throw tmn_exception::LogicException("Accessing an element by out of range index in dynamic array sequence");
    }
    
    return array[index];
}

template <typename T, class Allocator>
T& DynamicArray<T, Allocator>::operator[](std::size_t index) noexcept{
    return array[index];
}

template <typename T, class Allocator>
const T& DynamicArray<T, Allocator>::operator[](std::size_t index) const noexcept{
    return array[index];
}

template <typename T, class Allocator>
const T* DynamicArray<T, Allocator>::data() const noexcept{
    return array;
}

template <typename T, class Allocator>
T* DynamicArray<T, Allocator>::data() noexcept{
    return array;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator methods :

template <typename T, class Allocator>
typename DynamicArray<T, Allocator>::iterator DynamicArray<T, Allocator>::begin() noexcept {
    return iterator(array);
}

template <typename T, class Allocator>
typename DynamicArray<T, Allocator>::const_iterator DynamicArray<T, Allocator>::begin() const noexcept {
    return const_iterator(array);
}

template <typename T, class Allocator>
typename DynamicArray<T, Allocator>::const_iterator DynamicArray<T, Allocator>::cbegin() const noexcept {
    return const_iterator(array);
}

template <typename T, class Allocator>
typename DynamicArray<T, Allocator>::iterator DynamicArray<T, Allocator>::end() noexcept {
    return iterator(array + _size);
}

template <typename T, class Allocator>
typename DynamicArray<T, Allocator>::const_iterator DynamicArray<T, Allocator>::end() const noexcept {
    return const_iterator(array + _size);
}

template <typename T, class Allocator>
typename DynamicArray<T, Allocator>::const_iterator DynamicArray<T, Allocator>::cend() const noexcept {
    return const_iterator(array + _size);
}

template <typename T, class Allocator>
typename tmn_iterator::reverse_iterator<typename DynamicArray<T, Allocator>::iterator> DynamicArray<T, Allocator>::rbegin() noexcept {
    iterator iter(array + _size);
    return tmn_iterator::reverse_iterator<iterator>(iter);
}

template <typename T, class Allocator>
typename tmn_iterator::reverse_iterator<typename DynamicArray<T, Allocator>::iterator> DynamicArray<T, Allocator>::rend() noexcept {
    iterator iter(array);
    return tmn_iterator::reverse_iterator<iterator>(iter);
}

template <typename T, class Allocator>
typename DynamicArray<T, Allocator>::iterator DynamicArray<T, Allocator>::insert(iterator pos, const T& value) {
    if (_size == _capacity) {
        reserve(_capacity * 2);
    }
    std::size_t index = pos - begin();
    for (std::size_t i = _size; i > index; --i) {
        // new (array + i) T(*(array + i - 1));
        allocator_traits::construct(_alloc, array + i, *(array + i - 1));
    }
    // new (array + index) T(value);
    allocator_traits::construct(_alloc, array + index, value);
    ++_size;
    return iterator(array + index);
}

template <typename T, class Allocator>
typename DynamicArray<T, Allocator>::iterator DynamicArray<T, Allocator>::insert(iterator pos, size_type count, const T& value) {
    if (_size == _capacity) {
        reserve(_capacity * 2);
    }
    std::size_t index = pos - begin();
    for (std::size_t i = _size + count - 1; i >= index + count; --i) {
        // new (array + i) T(*(array + i - count));
        allocator_traits::construct(_alloc, array + i, *(array + i - count));
    }
    for (std::size_t i = index; i < index + count; ++i) {
        // new (array + i) T(value);
        allocator_traits::construct(_alloc, array + i, value);
    }
    _size += count;
    return iterator(array + index);
}

template <typename T, class Allocator>
typename DynamicArray<T, Allocator>::iterator DynamicArray<T, Allocator>::erase(iterator pos) {
    std::size_t index = pos - begin();
    for (std::size_t i = index; i < _size - 1; ++i) {
        // new (array + i) T(*(array + i + 1));
        allocator_traits::construct(_alloc, array + i, *(array + i + 1));
    }
    // (array + _size - 1)->~T();
    allocator_traits::construct(_alloc, array + _size - 1);
    --_size;
    return iterator(array + index);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

}