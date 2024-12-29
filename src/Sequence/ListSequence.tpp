#include "../../include/Sequence/ListSequence.hpp"
#include "../../include/Exceptions/LogicException.hpp"

namespace tmn {
namespace sequence {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator implementation :

template <typename T, class Allocator>
template <bool isConst>
ListSequence<T, Allocator>::common_iterator<isConst>& 
ListSequence<T, Allocator>::common_iterator<isConst>::operator=(const common_iterator<isConst>& other){
    ptr = other.ptr;
    return *this;
}

template <typename T, class Allocator>
template <bool isConst>
typename ListSequence<T, Allocator>::common_iterator<isConst>::conditional_ref
ListSequence<T, Allocator>::common_iterator<isConst>::operator*() const {
    return ptr->value;
}

template <typename T, class Allocator>
template <bool isConst>
typename ListSequence<T, Allocator>::common_iterator<isConst>::conditional_ptr
ListSequence<T, Allocator>::common_iterator<isConst>::operator->() const {
    return &(ptr->value);
}

template <typename T, class Allocator>
template <bool isConst>
bool ListSequence<T, Allocator>::common_iterator<isConst>::operator==(const common_iterator<isConst>& other) const {
    return ptr == other.ptr;
}

template <typename T, class Allocator>
template <bool isConst>
bool ListSequence<T, Allocator>::common_iterator<isConst>::operator!=(const common_iterator<isConst>& other) const {
    return ptr != other.ptr;
}

template <typename T, class Allocator>
template <bool isConst>
ListSequence<T, Allocator>::common_iterator<isConst>& 
ListSequence<T, Allocator>::common_iterator<isConst>::operator++() {
    ptr = ptr->next;
    return *this;
}

template <typename T, class Allocator>
template <bool isConst>
ListSequence<T, Allocator>::common_iterator<isConst> 
ListSequence<T, Allocator>::common_iterator<isConst>::operator++(int) {
   common_iterator<isConst> tmp(*this);
   ptr = ptr->next;
   return tmp;
}

template <typename T, class Allocator>
template <bool isConst>
ListSequence<T, Allocator>::common_iterator<isConst>& 
ListSequence<T, Allocator>::common_iterator<isConst>::operator--() {
    ptr = ptr->prev;
    return *this;
}

template <typename T, class Allocator>
template <bool isConst>
ListSequence<T, Allocator>::common_iterator<isConst> 
ListSequence<T, Allocator>::common_iterator<isConst>::operator--(int) {
   common_iterator<isConst> tmp(*this);
    ptr = ptr->prev;
   return tmp;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion :

template <typename T, class Allocator>
ListSequence<T, Allocator>::ListSequence(std::size_t _size) : _size(_size), dummy_node(new Node(T())) {
    if (_size == 0) {
        return;
    }

    Node* prev_node = dummy_node;
    for (std::size_t i = 0; i < _size; ++i) {
        Node* new_node = new Node(T(), dummy_node, prev_node);
        prev_node->next = new_node;
        prev_node = new_node;
    }

    dummy_node->prev = prev_node;
}

template <typename T, class Allocator>
ListSequence<T, Allocator>::ListSequence(std::size_t _size, const T& default_element) : _size(_size), dummy_node(new Node(T())) {
    if (_size == 0) {
        return;
    }

    Node* prev_node = dummy_node;
    for (std::size_t i = 0; i < _size; ++i) {
        Node* new_node = new Node(T(default_element), dummy_node, prev_node);
        prev_node->next = new_node;
        prev_node = new_node;
    }

    dummy_node->prev = prev_node;
}

template <typename T, class Allocator>
ListSequence<T, Allocator>::ListSequence(const ListSequence<T, Allocator>& other) : _size(other._size), dummy_node(new Node(T())) {
    if (other._size == 0) {
        return;
    }

    Node* current = other.dummy_node->next;
    Node* prev_node = dummy_node;
    for (std::size_t i = 0; i < _size; ++i) {
        Node* new_node = new Node(T(current->value), dummy_node, prev_node);
        prev_node->next = new_node;
        prev_node = new_node;
        current = current->next;
    }

    dummy_node->prev = prev_node;
}

template <typename T, class Allocator>
ListSequence<T, Allocator>::ListSequence(std::initializer_list<T> lst) :_size(lst.size()), dummy_node(new Node(T())) {
    if (_size == 0) {
        return;
    }
    
    Node* prev_node = dummy_node;
    for (auto it = lst.begin(); it != lst.end(); ++it) {
        Node* new_node = new Node(*it, dummy_node, prev_node);
        prev_node->next = new_node;
        prev_node = new_node;
    }

    dummy_node->prev = prev_node;
}

template <typename T, class Allocator>
void ListSequence<T, Allocator>::swap(ListSequence<T, Allocator>& other) {
    std::swap(dummy_node, other.dummy_node);
    std::swap(_size, other._size);
}

template <typename T, class Allocator>
ListSequence<T, Allocator>& ListSequence<T, Allocator>::operator=(const ListSequence<T, Allocator>& other) {
    ListSequence<T> tmp(other);
    swap(tmp);
    return *this;
}

template <typename T, class Allocator>
ListSequence<T, Allocator>::~ListSequence() {
    clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Capacity & size :

template <typename T, class Allocator>
std::size_t ListSequence<T, Allocator>::size() const noexcept {
    return _size;
}

template <typename T, class Allocator>
bool ListSequence<T, Allocator>::empty() const noexcept{
    return _size == 0;
}

template <typename T, class Allocator>
void ListSequence<T, Allocator>::resize(std::size_t new_size, const T& value) {
    if (new_size < _size) {
        while (_size > new_size) {
            pop_back();
        }
    } 
    else if (new_size > _size) {
        while (_size < new_size) {
            push_back(value);
        }
    }
}

template <typename T, class Allocator>
ListSequence<T, Allocator>& ListSequence<T, Allocator>::clear() {
    while (_size > 0) {
        pop_front();
    }

    return *this; 
}

template <typename T, class Allocator>
ListSequence<T, Allocator>& ListSequence<T, Allocator>::push_front(const T& value) {
    Node* new_node = new Node(value, dummy_node->next, dummy_node);

    dummy_node->next->prev = new_node; 
    dummy_node->next = new_node; 

    ++_size;
    return *this;
}

template <typename T, class Allocator>
ListSequence<T, Allocator>& ListSequence<T, Allocator>::push_back(const T& value) {
    Node* new_node = new Node(value, dummy_node, dummy_node->prev);

    dummy_node->prev->next = new_node; 
    dummy_node->prev = new_node; 

    ++_size;
    return *this;
}

template <typename T, class Allocator>
ListSequence<T, Allocator>& ListSequence<T, Allocator>::pop_front() {
    if (_size > 0) {
        Node* node_to_delete = dummy_node->next; 
        dummy_node->next = node_to_delete->next;
        node_to_delete->next->prev = dummy_node; 
        delete node_to_delete;
        --_size;
    }
    else{
        throw tmn::LogicException("Attempt to delete from an empty list sequence");
    }
    return *this; 
}

template <typename T, class Allocator>
ListSequence<T, Allocator>& ListSequence<T, Allocator>::pop_back() {
    if (_size > 0) {
        Node* node_to_delete = dummy_node->prev; 
        dummy_node->prev = node_to_delete->prev;
        node_to_delete->prev->next = dummy_node; 
        delete node_to_delete;
        --_size;
    }
    else{
        throw tmn::LogicException("Attempt to delete from an empty list sequence");
    }
    return *this; 
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Element access methods :


template <typename T, class Allocator>
const T& ListSequence<T, Allocator>::front() const {
    if (_size == 0){
        throw tmn::LogicException("Attempt to delete from an empty list sequence");
    }
    return dummy_node->next->value;
}

template <typename T, class Allocator>
T& ListSequence<T, Allocator>::front() {
    if (_size == 0){
        throw tmn::LogicException("Attempt to delete from an empty list sequence");
    }
    return dummy_node->next->value;
}

template <typename T, class Allocator>
const T& ListSequence<T, Allocator>::back() const {
    if (_size == 0){
        throw tmn::LogicException("Attempt to delete from an empty list sequence");
    }
    return dummy_node->prev->value;
}

template <typename T, class Allocator>
T& ListSequence<T, Allocator>::back() {
    if (_size == 0){
        throw tmn::LogicException("Attempt to delete from an empty list sequence");
    }
    return dummy_node->prev->value;
}

template <typename T, class Allocator>
ListSequence<T, Allocator>& ListSequence<T, Allocator>::set(std::size_t index, const T& item) {
    if (index >= _size) {
        throw tmn::LogicException("Accessing an element by out of range index in list sequence");
    }

    Node* current = dummy_node->next;
    for (std::size_t i = 0; i < index; ++i) {
        current = current->next;
    }

    current->value = item;
    return *this;
}

template <typename T, class Allocator>
T& ListSequence<T, Allocator>::get(std::size_t index) {
    if (index >= _size) {
        throw tmn::LogicException("Accessing an element by out of range index in list sequence");
    }

    Node* current = dummy_node->next;
    for (std::size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->value;
}

template <typename T, class Allocator>
const T& ListSequence<T, Allocator>::get(std::size_t index) const {
    if (index >= _size) {
        throw tmn::LogicException("Accessing an element by out of range index in list sequence");
    }
    
    Node* current = dummy_node->next;
    for (std::size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->value;
}

template <typename T, class Allocator>
T& ListSequence<T, Allocator>::operator[](std::size_t index) {
    return get(index);
}

template <typename T, class Allocator>
const T& ListSequence<T, Allocator>::operator[](std::size_t index) const {
    return get(index);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator methods :

template <typename T, class Allocator>
typename ListSequence<T, Allocator>::iterator ListSequence<T, Allocator>::begin() noexcept {
    return iterator(dummy_node->next);
}

template <typename T, class Allocator>
typename ListSequence<T, Allocator>::const_iterator ListSequence<T, Allocator>::begin() const noexcept {
    return const_iterator(dummy_node->next);
}

template <typename T, class Allocator>
typename ListSequence<T, Allocator>::const_iterator ListSequence<T, Allocator>::cbegin() const noexcept {
    return const_iterator(dummy_node->next);
}

template <typename T, class Allocator>
typename ListSequence<T, Allocator>::iterator ListSequence<T, Allocator>::end() noexcept {
    return iterator(dummy_node);
}

template <typename T, class Allocator>
typename ListSequence<T, Allocator>::const_iterator ListSequence<T, Allocator>::end() const noexcept {
    return const_iterator(dummy_node);
}

template <typename T, class Allocator>
typename ListSequence<T, Allocator>::const_iterator ListSequence<T, Allocator>::cend() const noexcept {
        return const_iterator(dummy_node);
}

template <typename T, class Allocator>
typename tmn::reverse_iterator<typename ListSequence<T, Allocator>::iterator> ListSequence<T, Allocator>::rbegin() noexcept {
    iterator iter(dummy_node);
    return tmn::reverse_iterator<iterator>(iter);
}

template <typename T, class Allocator>
typename tmn::reverse_iterator<typename ListSequence<T, Allocator>::iterator> ListSequence<T, Allocator>::rend() noexcept {
    iterator iter(dummy_node->next);
    return tmn::reverse_iterator<iterator>(iter);   
}

template <typename T, class Allocator>
typename ListSequence<T, Allocator>::iterator ListSequence<T, Allocator>::insert(iterator pos, const T& value) {
    if (pos == end()) {
        push_back(value);
        return end();
    }

    Node* current = pos.ptr;
    Node* new_node = new Node(value, current, current->prev);
    
    current->prev->next = new_node;
    current->prev = new_node;
    ++_size;
    return iterator(new_node);
}

template <typename T, class Allocator>
typename ListSequence<T, Allocator>::iterator ListSequence<T, Allocator>::insert(iterator pos, size_type count, const T& value) {
    if (pos == end()) {
        while (count > 0) {
            push_back(value);
            count--;
        }
        return end();
    }
    
    Node* current = pos.ptr;
    for (size_t i = 0; i < count; ++i) {
        Node* new_node = new Node(value, current, current->prev);
        current->prev->next = new_node;
        current->prev = new_node;
        ++_size;
    }
    return iterator(current);
}

template <typename T, class Allocator>
typename ListSequence<T, Allocator>::iterator ListSequence<T, Allocator>::erase(iterator pos) {
    if (pos == end()) {
        throw tmn::LogicException("Accessing an element by out of range index in list sequence");
    }
    
    Node* node_to_delete = pos.ptr;
    Node* next_node = node_to_delete->next;
    Node* prev_node = node_to_delete->prev;
    
    prev_node->next = next_node;
    next_node->prev = prev_node;
    
    delete node_to_delete;
    --_size;
    return iterator(next_node); 
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Other modifiers :

template <typename T, class Allocator>
void ListSequence<T, Allocator>::merge(ListSequence<T, Allocator>& other) {
    // ... (!)
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

}
}