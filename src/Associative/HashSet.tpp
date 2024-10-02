#include <utility>
#include <algorithm>

#include "../../include/Associative/HashSet.hpp"
#include "../../include/Exceptions/AllocException.hpp"
#include "../../include/Exceptions/LogicException.hpp"



namespace tmn_associative {

// Node implementation :
template <class Key>
HashSet<Key>::Node::Node(const Key& value, std::size_t cache) noexcept : value(value), cache(cache) { }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator implementation :

template <class Key>
template <bool isConst>
HashSet<Key>::common_iterator<isConst>& 
HashSet<Key>::common_iterator<isConst>::operator=(const common_iterator<isConst>& other){
    ptr = other.ptr;
    return *this;
}

template <class Key>
template <bool isConst>
typename HashSet<Key>::common_iterator<isConst>::conditional_ref
HashSet<Key>::common_iterator<isConst>::operator*() const {
    return ptr->value;
}

template <class Key>
template <bool isConst>
typename HashSet<Key>::common_iterator<isConst>::conditional_ptr
HashSet<Key>::common_iterator<isConst>::operator->() const {
    return &(ptr->value);
}

template <class Key>
template <bool isConst>
bool HashSet<Key>::common_iterator<isConst>::operator==(const common_iterator<isConst>& other) const {
    return ptr == other.ptr;
}

template <class Key>
template <bool isConst>
bool HashSet<Key>::common_iterator<isConst>::operator!=(const common_iterator<isConst>& other) const {
    return ptr != other.ptr;
}

template <class Key>
template <bool isConst>
HashSet<Key>::common_iterator<isConst>& 
HashSet<Key>::common_iterator<isConst>::operator++() {
    ptr = ptr->next;
    return *this;
}

template <class Key>
template <bool isConst>
HashSet<Key>::common_iterator<isConst> 
HashSet<Key>::common_iterator<isConst>::operator++(int) {
   common_iterator<isConst> tmp(*this);
   ptr = ptr->next;
   return tmp;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion :

template <class Key>
HashSet<Key>::HashSet() noexcept {
    _storage = reinterpret_cast<Node**>(new int8_t[_buffer_size * sizeof(Node*)]);
    std::fill(_storage, _storage + _buffer_size, nullptr);
}

template <class Key>
HashSet<Key>::HashSet(const HashSet<Key>& other) : _buffer_size(other._buffer_size){
    _storage = reinterpret_cast<Node**>(new int8_t[_buffer_size * sizeof(Node*)]);
    std::fill(_storage, _storage + _buffer_size, nullptr);

    try{
        for (const auto& value: other){
            insert(value);
        }
    }
    catch(...){
        clear();
        delete[] reinterpret_cast<int8_t*>(_storage);
        throw;
    }
}

template <class Key>
HashSet<Key>::HashSet(HashSet<Key>&& other) : 
    _storage(other._storage),
    _size(other._size), 
    _buffer_size(other._buffer_size),
    _alloc_node(other._alloc_node),
    _head(other._head)
{
    other._size = 0;
    other._buffer_size = 0;
    other._storage = nullptr;
    other._head = nullptr;
}

template <class Key>
HashSet<Key>::HashSet(std::initializer_list<Key> lst) : HashSet() {
    for (const auto& value : lst) {
        insert(value);
    }
}

template <class Key>
void HashSet<Key>::swap(HashSet<Key>& other) {
    std::swap(_size, other._size); 
    std::swap(_buffer_size, other._buffer_size);
    std::swap(_storage, other._storage);
    std::swap(_head, other._head);
    std::swap(_alloc_node, other._alloc_node);
}

template <class Key>
HashSet<Key>& HashSet<Key>::operator=(const HashSet<Key>& other) {
    clear();
    delete[] reinterpret_cast<int8_t*>(_storage);

    _storage = reinterpret_cast<Node**>(new int8_t[other._buffer_size * sizeof(Node*)]);
    std::fill(_storage, _storage + other._buffer_size, nullptr);

    try{
        for (const auto& value: other){
            insert(value);
        }
    }
    catch(...){
        clear();
        delete[] reinterpret_cast<int8_t*>(_storage);
        throw;
    }

    _buffer_size = other._buffer_size;
    _alloc_node = other._alloc_node;

    return *this;
}

template <class Key>
HashSet<Key>& HashSet<Key>::operator=(HashSet<Key>&& other) noexcept {
    clear();
    delete[] reinterpret_cast<int8_t*>(_storage);

    _size = other._size;
    _buffer_size = other._buffer_size;
    _storage = other._storage;
    _head = other._head;
    _alloc_node = other._alloc_node;

    other._size = 0;
    other._buffer_size = 0;
    other._storage = nullptr;
    other._head = nullptr;

    return *this;
}

template <class Key>
HashSet<Key>::~HashSet() {
    clear();
    delete[] reinterpret_cast<int8_t*>(_storage);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Capacity & size & get fields:

template<class Key>
std::size_t HashSet<Key>::size() const noexcept {
    return _size;
}

template<class Key>
std::size_t HashSet<Key>::buffer_size() const noexcept {
    return _buffer_size;
}

template<class Key>
bool HashSet<Key>::empty() const noexcept {
    return _size == 0;
}

template<class Key>
float HashSet<Key>::load_factor() const {
    return static_cast<float>(_size) / _buffer_size;
}

template<class Key>
float HashSet<Key>::max_load_factor() const {
    return _max_load_factor;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Modifiers :

template <class Key>
HashSet<Key>& HashSet<Key>::insert(const Key& value){
    if (_buffer_size == 0 || static_cast<float>(_size) / _buffer_size > _max_load_factor){
        rehash(_buffer_size * 2);
    }

    std::size_t hash_index = tmn_hash::Hash(value);
    
    Node* new_node = allocator_traits_node::allocate(_alloc_node, 1);
    allocator_traits_node::construct(_alloc_node, new_node, value, hash_index);
    
    hash_index %= _buffer_size;

    if (_storage[hash_index]){
        Node* current = _storage[hash_index];
        while (current && current->cache % _buffer_size == hash_index){
            if (current->value == value){
                allocator_traits_node::destroy(_alloc_node, new_node);
                allocator_traits_node::deallocate(_alloc_node, new_node, 1);
                return *this;
            }
            current = current->next;
        }

        new_node->prev = _storage[hash_index];
        new_node->next = _storage[hash_index]->next;
        if(_storage[hash_index]->next){
            _storage[hash_index]->next->prev = new_node;
        }
        _storage[hash_index]->next = new_node;
    }
    else{
        _storage[hash_index] = new_node;
        new_node->next = _head;
        if(_head){
            _head->prev = new_node;
        }

        _head = new_node;
    }

    ++_size;

    if (_buffer_size == 0 || static_cast<float>(_size) / _buffer_size > _max_load_factor){
        rehash(_buffer_size * 2);
    }
    
    return *this;
}

template <class Key>
HashSet<Key>& HashSet<Key>::insert(Key&& value) {
    if (_buffer_size == 0 || static_cast<float>(_size) / _buffer_size > _max_load_factor){
        rehash(_buffer_size * 2);
    }

    std::size_t hash_index = tmn_hash::Hash(value);
    
    Node* new_node = allocator_traits_node::allocate(_alloc_node, 1);
    allocator_traits_node::construct(_alloc_node, new_node, value, hash_index);
    
    hash_index %= _buffer_size;

    if (_storage[hash_index]){
        Node* current = _storage[hash_index];
        while (current && current->cache % _buffer_size == hash_index){
            if (current->value == value){
                allocator_traits_node::destroy(_alloc_node, new_node);
                allocator_traits_node::deallocate(_alloc_node, new_node, 1);
                return *this;
            }
            current = current->next;
        }

        new_node->prev = _storage[hash_index];
        new_node->next = _storage[hash_index]->next;
        if(_storage[hash_index]->next){
            _storage[hash_index]->next->prev = new_node;
        }
        _storage[hash_index]->next = new_node;
    }
    else{
        _storage[hash_index] = new_node;
        new_node->next = _head;
        if(_head){
            _head->prev = new_node;
        }

        _head = new_node;
    }

    ++_size;

    if (_buffer_size == 0 || static_cast<float>(_size) / _buffer_size > _max_load_factor){
        rehash(_buffer_size * 2);
    }
    
    return *this;
}

template <class Key>
void HashSet<Key>::erase_node(Node* to_remove) {
    if (to_remove == nullptr) return;

    if(to_remove->prev){
        to_remove->prev->next = to_remove->next;
    }
    else{
        _head = to_remove->next;
    }

    if(to_remove->next){
        to_remove->next->prev = to_remove->prev;
    }
    
    allocator_traits_node::destroy(_alloc_node, to_remove);
    allocator_traits_node::deallocate(_alloc_node, to_remove, 1);

    --_size;
    to_remove = nullptr;
}

template <class Key>
bool HashSet<Key>::erase(const Key& key){
    std::size_t hash_index = tmn_hash::Hash(key) % _buffer_size;

    if (_storage[hash_index]){
        if (_storage[hash_index]->value == key){
            erase_node(_storage[hash_index]);
            _storage[hash_index] = nullptr;
            return true;
        }
        
        Node* current = _storage[hash_index]->next;
        while (current && current->cache % _buffer_size == hash_index){
            if (current->value == key){
                erase_node(current);
                break;
            }
            current = current->next;
        }
    }

    return false;
}

template <class Key>
bool HashSet<Key>::erase(Key&& key){
    std::size_t hash_index = tmn_hash::Hash(key) % _buffer_size;

    if (_storage[hash_index]){
        if (_storage[hash_index]->value == key){
            erase_node(_storage[hash_index]);
            _storage[hash_index] = nullptr;
            return true;
        }
        
        Node* current = _storage[hash_index]->next;
        while (current && current->cache % _buffer_size == hash_index){
            if (current->value == key){
                erase_node(current);
                break;
            }
            current = current->next;
        }
    }

    return false;
}

template <class Key>
HashSet<Key>& HashSet<Key>::clear() noexcept {
    while (_size > 0) {
        erase_node(_head);
    }
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Element access methods :

template <class Key>
bool HashSet<Key>::contains(const Key& key) const noexcept {
    std::size_t hash_index = tmn_hash::Hash(key) % _buffer_size;

    if (_storage[hash_index]){
        if (_storage[hash_index]->value == key){
            return true;
        }
        Node* current = _storage[hash_index]->next;
        while (current && current->cache % _buffer_size == hash_index){
            if (current->value == key){
                return true;
            }
            current = current->next;
        }
    }
    
    return false;
}

template <class Key>
tmn_sequence::ArraySequence<Key> HashSet<Key>::to_sequence() const {
    tmn_sequence::ArraySequence<Key> keys;

    Node* current = _head;
    while (current) {
        keys.push_back(current->value);
        current = current->next;
    }

    return keys;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator methods :

template <class Key>
typename HashSet<Key>::iterator HashSet<Key>::begin() noexcept {
    return iterator(_head);
}

template <class Key>
typename HashSet<Key>::const_iterator HashSet<Key>::begin() const noexcept {
    return const_iterator(_head);
}

template <class Key>
typename HashSet<Key>::const_iterator HashSet<Key>::cbegin() const noexcept {
    return const_iterator(_head);
}

template <class Key>
typename HashSet<Key>::iterator HashSet<Key>::end() noexcept {
    return iterator(nullptr);
}

template <class Key>
typename HashSet<Key>::const_iterator HashSet<Key>::end() const noexcept {
    return const_iterator(nullptr);
}

template <class Key>
typename HashSet<Key>::const_iterator HashSet<Key>::cend() const noexcept {
    return const_iterator(nullptr);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Hash policy :

template <class Key>
void HashSet<Key>::rehash(std::size_t new_buffer_size) {
    if (new_buffer_size <= _buffer_size){ return; }

    Node** new_storage = reinterpret_cast<Node**>(new int8_t[new_buffer_size * sizeof(Node*)]);
    std::fill(new_storage, new_storage + new_buffer_size, nullptr);

    Node* current = _head;
    try{
        while (current){
            if (!new_storage[current->cache % new_buffer_size]){
                new_storage[current->cache % new_buffer_size] = current;
            }

            current = current->next;
        }
    }
    catch(...){
        delete[] reinterpret_cast<int8_t*>(new_storage);
        throw;
    }

    delete[] reinterpret_cast<int8_t*>(_storage);
    _storage = new_storage;
    _buffer_size = new_buffer_size;
}

template <class Key>
void HashSet<Key>::reserve(std::size_t new_buffer_size) {
    if (new_buffer_size <= _buffer_size){ return; }

    Node** new_storage = reinterpret_cast<Node**>(new int8_t[new_buffer_size * sizeof(Node*)]);
    std::fill(_storage, _storage + _buffer_size, nullptr);

    std::size_t i = 0;
    try{
        for (; i < _size; ++i){
            new_storage[i] = *(_storage + i);
        }
    }
    catch(...){
        delete[] reinterpret_cast<int8_t*>(new_storage);
        throw;
    }

    delete[] reinterpret_cast<int8_t*>(_storage);
    _storage = new_storage;
    _buffer_size = new_buffer_size;
}

}