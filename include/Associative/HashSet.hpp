#pragma once

#include <cstdint>
#include <memory>
#include <initializer_list>

#include "../Iterator/Iterator.hpp"
#include "../Optional/Optional.hpp"
#include "../Sequence/ArraySequence.hpp"
#include "Hash.hpp"
#include "../tmn.hpp"



namespace tmn {
namespace associative {

template <class Key>
class HashSet {
private:
    // Support structures and methods:
    struct Node {
        const Key value;

        Node* next = nullptr;
        Node* prev = nullptr;

        std::size_t cache = 0;

        Node(const Key& other_value, std::size_t cache) noexcept;
    };

    struct const_iterator {
    public:
        using layer_ptr = const Node*;

        using iterator_category	= tmn::forward_iterator_tag;
        using value_type = const Key;
        using const_pointer = const Key*;
        using const_reference = const Key&;

    private:
        layer_ptr ptr;

    public:
        const_iterator() : ptr(nullptr) {}
        explicit const_iterator(const layer_ptr ptr) : ptr(ptr) {}
        const_iterator(const const_iterator& other) : ptr(other.ptr) {}

        const_iterator& operator=(const const_iterator& other);

        const Key& operator*() const;

        bool operator==(const const_iterator& other) const;
        bool operator!=(const const_iterator& other) const;

        const_iterator& operator++();
        const_iterator operator++(int);
    };

    void erase_node(Node* to_remove);


private:
    // Fields :
    Node** _storage = nullptr;
    std::size_t _size = 0;
    std::size_t _buffer_size = 32;
    float _max_load_factor = 0.5;

    std::allocator<Node> _alloc_node;

    Node* _head = nullptr;

public:
    // Using's :
    using value_type = Key;
    using allocator_traits_node = std::allocator_traits<std::allocator<Node>>;

    // Constructors & assignment & conversion :
    HashSet() noexcept;
    HashSet(const HashSet<Key>& other);
    HashSet(HashSet<Key>&& other);
    HashSet(std::initializer_list<Key> lst);

    void swap(HashSet<Key>& other);
    HashSet& operator=(const HashSet<Key>& other);

    HashSet& operator=(HashSet<Key>&& other) noexcept;

    ~HashSet();

    // Capacity & size & get fields:
    std::size_t size() const noexcept;
    std::size_t buffer_size() const noexcept;
    bool empty() const noexcept;
    float load_factor() const;
    float max_load_factor() const;

    // Modifiers :
    HashSet& insert(const Key& value);
    HashSet& insert(Key&& value);

    bool erase(const Key& key);
    bool erase(Key&& key);

    HashSet& clear() noexcept;

    // Element access methods :
    bool contains(const Key& key) const noexcept;

    tmn::sequence::ArraySequence<Key> to_sequence() const;

    // Iterator methods :
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;

    // Hash policy :
    void rehash(std::size_t new_buffer_size);
	void reserve(std::size_t new_buffer_size);
};

}
}

#include "../../src/Associative/HashSet.tpp"
