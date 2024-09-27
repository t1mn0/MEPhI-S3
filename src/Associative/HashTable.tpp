#include "../../include/Associative/HashTable.hpp"



namespace tmn_associative {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion :

template <class Key, class Value, class CollisionStrategy>
HashTable<Key, Value, CollisionStrategy>::HashTable() {
    _storage = allocator_traits_node::allocate(_alloc_node, _buffer_size);
}

template <class Key, class Value, class CollisionStrategy>
HashTable<Key, Value, CollisionStrategy>::HashTable(std::size_t node_count) : _size(_size) {
    if (_size > _buffer_size) {
        _buffer_size = _size * 2;
    }
    
    _storage = allocator_traits_node::allocate(_alloc_node, _buffer_size);
}

template <class Key, class Value, class CollisionStrategy>
template <class OtherCollisionStrategy> 
HashTable<Key, Value, CollisionStrategy>::HashTable(const HashTable<Key, Value, OtherCollisionStrategy>& other) 
    : _size(other._size), _buffer_size(other._buffer_size) {
    
    _storage = allocator_traits_node::allocate(_alloc_node, _buffer_size);

    std::size_t i = 0;
    try{
        for (; i < _size; ++i){
            // TODO: Прошить ноды и скопировать ноды одной корзины
            allocator_traits::construct(_alloc_node, _storage + i, other._storage[i]);
        }
    }
    catch(...){
        // TODO: Удалять по прошитым нодам (итерация по нодам, а не по индексам)
        for (std::size_t j = 0; j < i; ++j){
            allocator_traits::destroy(_alloc_pair, _storage + j);
        }
        allocator_traits::deallocate(_alloc_pair, _storage, _buffer_size);
        throw;
    }
}



}