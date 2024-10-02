#include <utility>
#include <algorithm>

#include "../../include/Associative/HashTable.hpp"
#include "../../include/Exceptions/AllocException.hpp"
#include "../../include/Exceptions/LogicException.hpp"



namespace tmn_associative {

// Node implementation :
template <class Key, class Value>
HashTable<Key, Value>::Node::Node(const tmn::Pair<const Key, Value>& other_pair, std::size_t cache) noexcept : pair(other_pair), cache(cache) { }

template <class Key, class Value>
HashTable<Key, Value>::Node::Node(Node&& other) noexcept : pair(std::move(other.pair)), cache(std::move(other.cache)), next(other.next), prev(other.prev) {
    other.next = nullptr;
    other.prev = nullptr;
    other.cache = 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator implementation :

template <class Key, class Value>
template <bool isConst>
HashTable<Key, Value>::common_iterator<isConst>& 
HashTable<Key, Value>::common_iterator<isConst>::operator=(const common_iterator<isConst>& other){
    ptr = other.ptr;
    return *this;
}

template <class Key, class Value>
template <bool isConst>
typename HashTable<Key, Value>::common_iterator<isConst>::conditional_ref
HashTable<Key, Value>::common_iterator<isConst>::operator*() const {
    return ptr->pair;
}

template <class Key, class Value>
template <bool isConst>
typename HashTable<Key, Value>::common_iterator<isConst>::conditional_ptr
HashTable<Key, Value>::common_iterator<isConst>::operator->() const {
    return &(ptr->pair);
}

template <class Key, class Value>
template <bool isConst>
bool HashTable<Key, Value>::common_iterator<isConst>::operator==(const common_iterator<isConst>& other) const {
    return ptr == other.ptr;
}

template <class Key, class Value>
template <bool isConst>
bool HashTable<Key, Value>::common_iterator<isConst>::operator!=(const common_iterator<isConst>& other) const {
    return ptr != other.ptr;
}

template <class Key, class Value>
template <bool isConst>
HashTable<Key, Value>::common_iterator<isConst>& 
HashTable<Key, Value>::common_iterator<isConst>::operator++() {
    ptr = ptr->next;
    return *this;
}

template <class Key, class Value>
template <bool isConst>
HashTable<Key, Value>::common_iterator<isConst> 
HashTable<Key, Value>::common_iterator<isConst>::operator++(int) {
   common_iterator<isConst> tmp(*this);
   ptr = ptr->next;
   return tmp;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion :

template <class Key, class Value>
HashTable<Key, Value>::HashTable() noexcept {
    _storage = reinterpret_cast<Node**>(new int8_t[_buffer_size * sizeof(Node*)]);
    std::fill(_storage, _storage + _buffer_size, nullptr);
}

template <class Key, class Value>
HashTable<Key, Value>::HashTable(const HashTable<Key, Value>& other) : _buffer_size(other._buffer_size){
    _storage = reinterpret_cast<Node**>(new int8_t[_buffer_size * sizeof(Node*)]);
    std::fill(_storage, _storage + _buffer_size, nullptr);

    try{
        for (const auto& pair: other){
            insert(pair);
        }
    }
    catch(...){
        clear();
        delete[] reinterpret_cast<int8_t*>(_storage);
        throw;
    }
}

template <class Key, class Value>
HashTable<Key, Value>::HashTable(HashTable<Key, Value>&& other) : 
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

template <class Key, class Value>
HashTable<Key, Value>::HashTable(std::initializer_list<tmn::Pair<const Key, Value>> lst) : HashTable() {
    for (const auto& pair : lst) {
        insert(pair);
    }
}

template <class Key, class Value>
void HashTable<Key, Value>::swap(HashTable<Key, Value>& other) {
    std::swap(_size, other._size); 
    std::swap(_buffer_size, other._buffer_size);
    std::swap(_storage, other._storage);
    std::swap(_head, other._head);
    std::swap(_alloc_node, other._alloc_node);
}

template <class Key, class Value>
HashTable<Key, Value>& HashTable<Key, Value>::operator=(const HashTable<Key, Value>& other) {
    clear();
    delete[] reinterpret_cast<int8_t*>(_storage);

    _storage = reinterpret_cast<Node**>(new int8_t[other._buffer_size * sizeof(Node*)]);
    std::fill(_storage, _storage + other._buffer_size, nullptr);

    try{
        for (const auto& pair: other){
            insert(pair);
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

template <class Key, class Value>
HashTable<Key, Value>& HashTable<Key, Value>::operator=(HashTable<Key, Value>&& other) noexcept {
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

template <class Key, class Value>
HashTable<Key, Value>::~HashTable() {
    clear();
    delete[] reinterpret_cast<int8_t*>(_storage);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Capacity & size & get fields:

template<class Key, class Value>
std::size_t HashTable<Key, Value>::size() const noexcept {
    return _size;
}

template<class Key, class Value>
std::size_t HashTable<Key, Value>::buffer_size() const noexcept {
    return _buffer_size;
}

template<class Key, class Value>
bool HashTable<Key, Value>::empty() const noexcept {
    return _size == 0;
}

template<class Key, class Value>
float HashTable<Key, Value>::load_factor() const {
    return static_cast<float>(_size) / _buffer_size;
}

template<class Key, class Value>
float HashTable<Key, Value>::max_load_factor() const {
    return _max_load_factor;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Modifiers :

template <class Key, class Value>
HashTable<Key, Value>& HashTable<Key, Value>::insert(const tmn::Pair<const Key, Value>& pair){
    if (_buffer_size == 0 || static_cast<float>(_size) / _buffer_size > _max_load_factor){
        rehash(_buffer_size * 2);
    }

    std::size_t hash_index = tmn_hash::Hash(pair.first);
    
    Node* new_node = allocator_traits_node::allocate(_alloc_node, 1);
    allocator_traits_node::construct(_alloc_node, new_node, pair, hash_index);
    
    hash_index %= _buffer_size;

    if (_storage[hash_index]){
        Node* current = _storage[hash_index];
        while (current && current->cache % _buffer_size == hash_index){
            if (current->pair.first == pair.first){
                current->pair.second = pair.second;
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

template <class Key, class Value>
HashTable<Key, Value>& HashTable<Key, Value>::insert(tmn::Pair<const Key, Value>&& pair) {
    if (_buffer_size == 0 || static_cast<float>(_size) / _buffer_size > _max_load_factor){
        rehash(_buffer_size * 2);
    }

    std::size_t hash_index = tmn_hash::Hash(pair.first);
    
    Node* new_node = allocator_traits_node::allocate(_alloc_node, 1);
    allocator_traits_node::construct(_alloc_node, new_node, pair, hash_index);
    
    hash_index %= _buffer_size;

    if (_storage[hash_index]){
        Node* current = _storage[hash_index];
        while (current && current->cache % _buffer_size == hash_index){
            if (current->pair.first == pair.first){
                current->pair.second = pair.second;
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

template <class Key, class Value>
void HashTable<Key, Value>::erase_node(Node* to_remove) {
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

template <class Key, class Value>
bool HashTable<Key, Value>::erase(const Key& key){
    std::size_t hash_index = tmn_hash::Hash(key) % _buffer_size;

    if (_storage[hash_index]){
        if (_storage[hash_index]->pair.first == key){
            erase_node(_storage[hash_index]);
            _storage[hash_index] = nullptr;
            return true;
        }
        
        Node* current = _storage[hash_index]->next;
        while (current && current->cache % _buffer_size == hash_index){
            if (current->pair.first == key){
                erase_node(current);
                break;
            }
            current = current->next;
        }
    }

    return false;
}

template <class Key, class Value>
bool HashTable<Key, Value>::erase(Key&& key){
    std::size_t hash_index = tmn_hash::Hash(key) % _buffer_size;

    if (_storage[hash_index]){
        if (_storage[hash_index]->pair.first == key){
            erase_node(_storage[hash_index]);
            _storage[hash_index] = nullptr;
            return true;
        }
        
        Node* current = _storage[hash_index]->next;
        while (current && current->cache % _buffer_size == hash_index){
            if (current->pair.first == key){
                erase_node(current);
                break;
            }
            current = current->next;
        }
    }

    return false;
}

template <class Key, class Value>
HashTable<Key, Value>& HashTable<Key, Value>::clear() noexcept {
    while (_size > 0) {
        erase_node(_head);
    }
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Element access methods :

template <class Key, class Value>
tmn::Optional<Value> HashTable<Key, Value>::get(const Key& key) const noexcept {
    std::size_t hash_index = tmn_hash::Hash(key) % _buffer_size;

    if (_storage[hash_index]){
        if (_storage[hash_index]->pair.first == key){
            return tmn::Optional<Value>(_storage[hash_index]->pair.second);
        }
        Node* current = _storage[hash_index]->next;
        while (current && current->cache % _buffer_size == hash_index){
            if (current->pair.first == key){
                return tmn::Optional<Value>(_storage[hash_index]->pair.second);
                break;
            }
            current = current->next;
        }
    }

    return tmn::Optional<Value>();
}

template <class Key, class Value>
tmn::Optional<Value> HashTable<Key, Value>::get(Key&& key) const noexcept {
    std::size_t hash_index = tmn_hash::Hash(key) % _buffer_size;

    if (_storage[hash_index]){
        if (_storage[hash_index]->pair.first == key){
            return tmn::Optional<Value>(_storage[hash_index]->pair.second);
        }
        Node* current = _storage[hash_index]->next;
        while (current && current->cache % _buffer_size == hash_index){
            if (current->pair.first == key){
                return tmn::Optional<Value>(_storage[hash_index]->pair.second);
                break;
            }
            current = current->next;
        }
    }

    return tmn::Optional<Value>();
}

template <class Key, class Value>
const Value& HashTable<Key, Value>::operator[](const Key& key) const {
    std::size_t hash_index = tmn_hash::Hash(key) % _buffer_size;

    if (_storage[hash_index]){
        if (_storage[hash_index]->pair.first == key){
            return _storage[hash_index]->pair.second;
        }
        Node* current = _storage[hash_index]->next;
        while (current && current->cache % _buffer_size == hash_index){
            if (current->pair.first == key){
                return _storage[hash_index]->pair.second;
                break;
            }
            current = current->next;
        }
    }

    throw tmn_exception::LogicException("Element with stated key in the table is missing [HashTable<Key, Value>::get]");
}

template <class Key, class Value>
Value& HashTable<Key, Value>::operator[](const Key& key) {
    std::size_t hash_index = tmn_hash::Hash(key) % _buffer_size;

    if (_storage[hash_index]){
        if (_storage[hash_index]->pair.first == key){
            return _storage[hash_index]->pair.second;
        }
        Node* current = _storage[hash_index]->next;
        while (current && current->cache % _buffer_size == hash_index){
            if (current->pair.first == key){
                return _storage[hash_index]->pair.second;
                break;
            }
            current = current->next;
        }
    }

    throw tmn_exception::LogicException("Element with stated key in the table is missing [HashTable<Key, Value>::get]");
}

template <class Key, class Value>
bool HashTable<Key, Value>::contains(const Key& key) const {
    std::size_t hash_index = tmn_hash::Hash(key) % _buffer_size;

    if (_storage[hash_index]){
        if (_storage[hash_index]->pair.first == key){
            return true;
        }
        Node* current = _storage[hash_index]->next;
        while (current && current->cache % _buffer_size == hash_index){
            if (current->pair.first == key){
                return true;
            }
            current = current->next;
        }
    }
    
    return false;
}

template <class Key, class Value>
tmn_sequence::ArraySequence<Key> HashTable<Key, Value>::keys() const {
    tmn_sequence::ArraySequence<Key> keys;

    Node* current = _head;
    while (current) {
        keys.push_back(current->pair.first);
        current = current->next;
    }

    return keys;
}

template <class Key, class Value>
tmn_sequence::ArraySequence<Value> HashTable<Key, Value>::values() const {
    tmn_sequence::ArraySequence<Value> values;

    Node* current = _head;
    while (current) {
        values.push_back(current->pair.second);
        current = current->next;
    }

    return values;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator methods :

template <class Key, class Value>
typename HashTable<Key, Value>::iterator HashTable<Key, Value>::begin() noexcept {
    return iterator(_head);
}

template <class Key, class Value>
typename HashTable<Key, Value>::const_iterator HashTable<Key, Value>::begin() const noexcept {
    return const_iterator(_head);
}

template <class Key, class Value>
typename HashTable<Key, Value>::const_iterator HashTable<Key, Value>::cbegin() const noexcept {
    return const_iterator(_head);
}

template <class Key, class Value>
typename HashTable<Key, Value>::iterator HashTable<Key, Value>::end() noexcept {
    return iterator(nullptr);
}

template <class Key, class Value>
typename HashTable<Key, Value>::const_iterator HashTable<Key, Value>::end() const noexcept {
    return const_iterator(nullptr);
}

template <class Key, class Value>
typename HashTable<Key, Value>::const_iterator HashTable<Key, Value>::cend() const noexcept {
    return const_iterator(nullptr);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Hash policy :

template <class Key, class Value>
void HashTable<Key, Value>::rehash(std::size_t new_buffer_size) {
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

template <class Key, class Value>
void HashTable<Key, Value>::reserve(std::size_t new_buffer_size) {
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

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Buckets :

template <class Key, class Value>
std::size_t HashTable<Key, Value>::bucket_size(std::size_t index) const {
    std::size_t res = 0;
    Node* current = _storage[index];
    while (current && current->cache % _buffer_size == index){
        ++res;
        current = current->next;
    }
    return res;
}

}