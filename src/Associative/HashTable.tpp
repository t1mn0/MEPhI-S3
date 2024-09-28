#include "../../include/Associative/HashTable.hpp"

namespace tmn_associative {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Iterator implementation :

template <class Key, class Value, class CollisionStrategy>
template <bool isConst>
HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>& 
HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>::operator=(const common_iterator<isConst>& other){
    ptr = other.ptr;
    return *this;
}

template <class Key, class Value, class CollisionStrategy>
template <bool isConst>
typename HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>::conditional_ref
HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>::operator*() const {
    return ptr->pair;
}

template <class Key, class Value, class CollisionStrategy>
template <bool isConst>
typename HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>::conditional_ptr
HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>::operator->() const {
    return &(ptr->pair);
}

template <class Key, class Value, class CollisionStrategy>
template <bool isConst>
bool HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>::operator==(const common_iterator<isConst>& other) const {
    return ptr == other.ptr;
}

template <class Key, class Value, class CollisionStrategy>
template <bool isConst>
bool HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>::operator!=(const common_iterator<isConst>& other) const {
    return ptr != other.ptr;
}

template <class Key, class Value, class CollisionStrategy>
template <bool isConst>
HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>& 
HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>::operator++() {
    if (ptr->down){
        ptr = ptr->down;
    }
    else{
        ptr = ptr->right;
    }
    return *this;
}

template <class Key, class Value, class CollisionStrategy>
template <bool isConst>
HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst> 
HashTable<Key, Value, CollisionStrategy>::common_iterator<isConst>::operator++(int) {
   common_iterator<isConst> tmp(*this);
   if (ptr->down){
        ptr = ptr->down;
    }
    else{
        ptr = ptr->right;
    }
   return tmp;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion :

template <class Key, class Value, class CollisionStrategy>
HashTable<Key, Value, CollisionStrategy>::HashTable() {
    _storage = allocator_traits::allocate(_alloc_node, _buffer_size);
}

template <class Key, class Value, class CollisionStrategy>
HashTable<Key, Value, CollisionStrategy>::HashTable(std::size_t node_count) : _size(_size) {
    if (_size > _buffer_size) {
        _buffer_size = _size * 2;
    }
    
    _storage = allocator_traits::allocate(_alloc_node, _buffer_size);
}

template <class Key, class Value, class CollisionStrategy>
template <class OtherCollisionStrategy> 
HashTable<Key, Value, CollisionStrategy>::HashTable(const HashTable<Key, Value, OtherCollisionStrategy>& other) 
    : _size(other._size), _buffer_size(other._buffer_size) {
    
    _storage = allocator_traits::allocate(_alloc_node, _buffer_size);

    std::size_t i = 0;
    try{
        for (auto iter = other.begin(); iter != other.end(); ++iter, ++i){
            allocator_traits::construct(_alloc_node, _storage + i, *iter);
        }
    }
    catch(...){
        for (std::size_t j = 0; j < i; ++j){
            allocator_traits::destroy(_alloc_node, _storage + j);
        }
        allocator_traits::deallocate(_alloc_node, _storage, _buffer_size);
        throw;
    }
}

template <class Key, class Value, class CollisionStrategy>
HashTable<Key, Value, CollisionStrategy>::~HashTable() {
    clear();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Capacity & size & get fields:

template<class Key, class Value, class CollisionStrategy>
std::size_t HashTable<Key, Value, CollisionStrategy>::size() const noexcept {
    return _size;
}

template<class Key, class Value, class CollisionStrategy>
std::size_t HashTable<Key, Value, CollisionStrategy>::buffer_size() const noexcept {
    return _buffer_size;
}

template<class Key, class Value, class CollisionStrategy>
bool HashTable<Key, Value, CollisionStrategy>::empty() const noexcept {
    return _size == 0;
}

template<class Key, class Value, class CollisionStrategy>
float HashTable<Key, Value, CollisionStrategy>::load_factor() const {
    return static_cast<_size> / _buffer_size;
}

template<class Key, class Value, class CollisionStrategy>
float HashTable<Key, Value, CollisionStrategy>::max_load_factor() const {
    return max_load_factor;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Modifiers :

template <class Key, class Value, class CollisionStrategy>
auto HashTable<Key, Value, CollisionStrategy>::insert(const tmn::Pair<const Key, Value>& pair)
    -> HashTable<Key, Value, CollisionStrategy>& 
{
    std::size_t hash_index = tmn_hash::Hash(pair.first) % _buffer_size;
    Node* memplace = collision_strategy_traits::get_memplace(_collision_strategy, _storage, _buffer_size, hash_index);
    allocator_traits::construct(_alloc_node, memplace, pair);
    _size++;

    if (collision_strategy_traits::is_horizontally()){
        if (_size == 1){
            _head = memplace;
            return *this;
        }
        else{
            // Горизонтальная прошивка: найти левый близкий узел и поставить ему в поле right наш новый узел. 
            // Если слева от _storage[hash_index] ничего нет, то ставим его в _head и ищем ему right в правой стороне
        }
    }
}

}