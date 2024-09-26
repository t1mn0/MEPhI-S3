#pragma once

#include <cstdint>
#include <memory>

#include "../Iterator/Iterator.hpp"
#include "../Pair/Pair.hpp"
#include "../tmn_strategy.hpp"
#include "../tmn.hpp"



namespace tmn_associative {

template <class Key, class Value, 
    class Allocator = std::allocator<tmn::Pair<const Key, Value>>,
    class CollisionStrategy = tmn_strategy::SeparateChaining>
class HashTable {
private:
    // Support structures :
    struct Node {
        Key key;
        Value value;

        Node* next = nullptr;
        Node* prev = nullptr;

        bool isBucket = false;

        Node(const Key& key, const T& value) : key(key), value(value){}
        Node(const Key& key, const T& value, Node* next, Node* prev) : key(key), value(value), next(next), prev(prev) {}
    };

    template <bool isConst>
    struct common_iterator {
    public:
        using conditional_layer_ptr = tmn::conditional_t<isConst, const Node*, Node*>;
        using conditional_ptr = tmn::conditional_t<isConst, const tmn::Pair<const Key, Value>*, tmn::Pair<const Key, Value>*>;
        using conditional_ref = tmn::conditional_t<isConst, const tmn::Pair<const Key, Value>&, tmn::Pair<const Key, Value>&>;
        using iterator_category	= tmn_iterator::forward_iterator_tag;
        using value_type = Node;
        using pointer = Node*;
        using const_pointer = const Node*;
        using reference = Node&;
        using const_reference = const Node&;
    
    private:
        conditional_layer_ptr ptr;

    public:
        common_iterator() : ptr(nullptr) {}
        explicit common_iterator(const conditional_layer_ptr ptr) : ptr(ptr) {}
        common_iterator(const common_iterator<isConst>& other) : ptr(other.ptr) {}

        common_iterator<isConst>& operator=(const common_iterator<isConst>& other);

        conditional_ref operator*() const;
        conditional_ptr operator->() const;

        bool operator==(const common_iterator<isConst>& other) const;
        bool operator!=(const common_iterator<isConst>& other) const;

        common_iterator<isConst>& operator++();
        common_iterator<isConst> operator++(int);
        common_iterator<isConst>& operator--();
        common_iterator<isConst> operator--(int);
        common_iterator<isConst>& operator+=(std::size_t n);
        common_iterator<isConst>& operator-=(std::size_t n);
    };


private:
    // Fields :
    Node** _storage = nullptr;
    std::size_t _storage_size = 0;
    std::size_t _size = 0;
    std::size_t _buffer_size = 4096;
    float _max_load_factor = 0.25;
    Allocator _alloc;

public:
    // Using's :
    using key_type = Key;
    using mapped_type = Value;
    using value_type = tmn::Pair<const Key, Value>;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using allocator_traits = std::allocator_traits<Allocator>;

    // Constructors & assignment & conversion :
    HashTable();
    explicit HashTable(size_type bucket_count);
    HashTable(const HashTable<Key, Value, Allocator, CollisionStrategy>& other);
    HashTable(HashTable<Key, Value, Allocator, CollisionStrategy>&& other);

    void swap(HashTable<Key, Value, Allocator, CollisionStrategy>& other);
    auto operator=(const HashTable<Key, Value, Allocator, CollisionStrategy>& other)
        -> HashTable<Key, Value, Allocator, CollisionStrategy>&;

    ~HashTable();


    // Capacity & size :
    std::size_t size() const noexcept;
    bool empty() const noexcept;

    // Modifiers :
    HashTable<Key, Value, Allocator, CollisionStrategy>& insert(const value_type& value);
    HashTable<Key, Value, Allocator, CollisionStrategy>& insert(value_type&& value);

    template<class... Args>
    HashTable<Key, Value, Allocator, CollisionStrategy>& emplace(Args&&... args);

    HashTable<Key, Value, Allocator, CollisionStrategy>& erase(const Key& key);
    
    template<class K>
    HashTable<Key, Value, Allocator, CollisionStrategy>& erase(K&& x);

    HashTable<Key, Value, Allocator, CollisionStrategy>& clear() noexcept;

    // Element access methods :
    T& get(const Key& key);
    const T& get(const Key& key) const;

    T& operator[](const Key& key);
    T& operator[](Key&& key);

    size_type count(const Key& key) const;

    bool contains(const Key& key) const;

    // Iterator methods :
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;  
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // Hash policy :
    float avg_load_factor() const;
    size_type max_load_factor() const;
    void rehash(size_type count);
	void reserve(size_type count);

    // Buckets :
    size_type bucket_count() const;
    size_type bucket_size(size_type n) const;
    size_type bucket(const Key& key) const;
};

}