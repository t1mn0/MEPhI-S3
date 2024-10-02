#pragma once

#include <cstdint>
#include <memory>
#include <initializer_list>
#include <concepts>

#include "../Iterator/Iterator.hpp"
#include "../Pair/Pair.hpp"
#include "../Optional/Optional.hpp"
#include "../Sequence/ArraySequence.hpp"
#include "Hash.hpp"
#include "../tmn.hpp"



template <typename T>
concept EqualityComparable = requires(T a, T b) {
  { a == b } -> std::convertible_to<bool>;
  { a != b } -> std::convertible_to<bool>;
};


// TODO : закольцевать двусвязный список (?)

namespace tmn_associative {

template <class Key, class Value>
class HashTable {
private:
    // Support structures and methods:
    struct Node {
        tmn::Pair<const Key, Value> pair;

        Node* next = nullptr;
        Node* prev = nullptr;
        
        std::size_t cache = 0;

        Node(const tmn::Pair<const Key, Value>& other_pair, std::size_t cache) noexcept;
        Node(Node&&) noexcept;
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
    };

    void erase_node(Node* to_remove);


private:
    // Fields :
    Node** _storage = nullptr;
    std::size_t _size = 0;
    std::size_t _buffer_size = 256;
    float _max_load_factor = 0.5;

    std::allocator<Node> _alloc_node;

    Node* _head = nullptr;

public:
    // Using's :
    using key_type = Key;
    using mapped_type = Value;
    using value_type = tmn::Pair<const Key, Value>;
    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using allocator_traits_node = std::allocator_traits<std::allocator<Node>>;

    // Constructors & assignment & conversion :
    HashTable() noexcept;
    HashTable(const HashTable<Key, Value>& other);
    HashTable(HashTable<Key, Value>&& other);
    HashTable(std::initializer_list<tmn::Pair<const Key, Value>> lst);

    void swap(HashTable<Key, Value>& other);
    HashTable& operator=(const HashTable<Key, Value>& other); 
    // operator=(const HashTable<Key, Value>&) have `No exception guarantee`.
    // It is solved by copy-pasting insert for new_storage (new allocated memory block)

    HashTable& operator=(HashTable&& other) noexcept;

    ~HashTable();

    // Capacity & size & get fields:
    std::size_t size() const noexcept;
    std::size_t buffer_size() const noexcept;
    bool empty() const noexcept;
    float load_factor() const;
    float max_load_factor() const;

    // Modifiers :
    HashTable& insert(const tmn::Pair<const Key, Value>& value);
    HashTable& insert(tmn::Pair<const Key, Value>&& value);

    bool erase(const Key& key);
    bool erase(Key&& key);

    HashTable& clear() noexcept;

    // Element access methods :
    tmn::Optional<Value> get(const Key& key) const noexcept;
    tmn::Optional<Value> get(Key&& key) const noexcept;

    const Value& operator[](const Key& key) const;
    Value& operator[](const Key& key);

    bool contains(const Key& key) const;

    tmn_sequence::ArraySequence<Key> keys() const;
    tmn_sequence::ArraySequence<Value> values() const;

    // Iterator methods :
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;  
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // Hash policy :
    void rehash(std::size_t new_buffer_size);
	void reserve(std::size_t new_buffer_size);

    // Buckets :
    std::size_t bucket_size(std::size_t index) const;
};



}



#include "../../src/Associative/HashTable.tpp"