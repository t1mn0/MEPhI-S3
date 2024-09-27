#pragma once

#include <cstdint>
#include <memory>
#include <concepts>

#include "../Iterator/Iterator.hpp"
#include "../Pair/Pair.hpp"
#include "../Optional/Optional.hpp"
#include "../tmn_strategy.hpp"
#include "../tmn.hpp"



template <typename T>
concept EqualityComparable = requires(T a, T b) {
  { a == b } -> std::convertible_to<bool>;
  { a != b } -> std::convertible_to<bool>;
};



namespace tmn_associative {



template <class Key, class Value, class CollisionStrategy = tmn_strategy::SeparateChaining<Key, Value>>
requires EqualityComparable<Key>

class HashTable {
private:
    // Support structures :
    // TODO : проработать копирование и создание
    struct Node {
        tmn::Pair<const Key, Value> pair;

        Node* next = nullptr;
        Node* prev = nullptr;

        std::size_t index = 0;
        bool isBucket = false;

        Node(const tmn::Pair<const Key, Value>& other) : pair(other) {}
    };

    template <bool isConst>
    struct common_iterator {
    public:
        using conditional_layer_ptr = tmn::conditional_t<isConst, const Node*, Node*>;
        using conditional_ptr = tmn::conditional_t<isConst, const tmn::Pair<const Key, Value>*, tmn::Pair<const Key, Value>*>;
        using conditional_ref = tmn::conditional_t<isConst, const tmn::Pair<const Key, Value>&, tmn::Pair<const Key, Value>&>;
        using iterator_category	= tmn_iterator::forward_iterator_tag;
        using value_type = tmn::Pair<const Key, Value>;
        using pointer = tmn::Pair<const Key, Value>*;
        using const_pointer = const tmn::Pair<const Key, Value>*;
        using reference = tmn::Pair<const Key, Value>&;
        using const_reference = const tmn::Pair<const Key, Value>&;
    
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
    std::size_t _size = 0;
    std::size_t _buffer_size = 256;
    float _max_load_factor = 0.25;

    std::allocator<tmn::Pair<const Key, Value>> _alloc_pair;
    std::allocator<Node> _alloc_node;

public:
    // Using's :
    using key_type = Key;
    using mapped_type = Value;
    using value_type = tmn::Pair<const Key, Value>;
    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using allocator_traits_pair = std::allocator_traits<std::allocator<tmn::Pair<const Key, Value>>;
    using allocator_traits_node = std::allocator_traits<std::allocator<Node>>;

    // Constructors & assignment & conversion :
    HashTable();
    explicit HashTable(std::size_t node_count);

    template <typename OtherCollisionStrategy>
    HashTable(const HashTable<Key, Value, OtherCollisionStrategy>& other);

    // template <typename OtherCollisionStrategy>
    // HashTable(HashTable<Key, Value, OtherCollisionStrategy>&& other);

    // HashTable(std::initializer_list<tmn::Pair<Key, Value>> lst);

    // void swap(HashTable<Key, Value, CollisionStrategy>& other);
    // auto operator=(const HashTable<Key, Value, CollisionStrategy>& other)
    //     -> HashTable<Key, Value, CollisionStrategy>&;

    ~HashTable();

    // Capacity & size & get fields:
    std::size_t size() const noexcept;
    std::size_t buffer_size() const noexcept;
    bool empty() const noexcept;
    float avg_load_factor() const;
    std::size_t max_load_factor() const;

    // Modifiers :
    HashTable<Key, Value, CollisionStrategy>& insert(const value_type& value);
    //HashTable<Key, Value, CollisionStrategy>& insert(value_type&& value);

    //template<class... Args>
    //HashTable<Key, Value, CollisionStrategy>& emplace(Args&&... args);

    HashTable<Key, Value, CollisionStrategy>& erase(const Key& key);
    
    //template<class K>
    //HashTable<Key, Value, CollisionStrategy>& erase(K&& x);

    HashTable<Key, Value, CollisionStrategy>& clear() noexcept;

    // Element access methods :
    Value& get(const Key& key);
    //const Value& get(const Key& key) const;

    // tmn::Optional<Value> operator[](const Key& key) const noexcept;
    // tmn::Optional<Value>& operator[](Key&& key) noexcept;

    //std::size_t count(const Key& key) const;

    bool contains(const Key& key) const;

    //tmn::Optional<tmn::Pair<Key*, std::size_t>*> keys() const;
    //tmn::Optional<tmn::Pair<Value*, std::size_t>*> value() const;

    // Iterator methods :
    // iterator begin() noexcept;
    // const_iterator begin() const noexcept;
    // const_iterator cbegin() const noexcept;  
    // iterator end() noexcept;
    // const_iterator end() const noexcept;
    // const_iterator cend() const noexcept;

    // Hash policy :
    // void rehash(size_type count);
	// void reserve(size_type count);

    // Buckets :
    // size_type bucket_count() const;
    // size_type bucket_size(size_type n) const;
    // size_type bucket(const Key& key) const;
};



}



#include "../../src/Associative/HashTable.tpp"