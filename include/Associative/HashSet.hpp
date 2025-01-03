#pragma once

#include <cstdint>
#include <memory>
#include <initializer_list>

#include "../Iterator/Iterator.hpp"
#include "../Optional/Optional.hpp"
#include "../Sequence/ArraySequence.hpp"
#include "Hash.hpp"
#include "../tmn.hpp"

namespace tmn{
namespace associative {

template <class Key>
class HashSet {
private:
    // Support structures and methods:
    struct Node {
        Key value;

        Node* next = nullptr;
        Node* prev = nullptr;

        std::size_t cache = 0;

        Node(const Key& other_value, std::size_t cache) noexcept;
    };

    template <bool isConst>
    struct common_iterator {
    public:
        using conditional_layer_ptr = tmn::conditional_t<isConst, const Node*, Node*>;
        using conditional_ptr = tmn::conditional_t<isConst, const Key*, Key*>;
        using conditional_ref = tmn::conditional_t<isConst, const Key&, Key&>;

        using iterator_category	= tmn::iterator::forward_iterator_tag;
        using value_type = Key;
        using pointer = Key*;
        using const_pointer = const Key*;
        using reference = Key&;
        using const_reference = const Key&;

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
    std::size_t _buffer_size = 32;
    float _max_load_factor = 0.5;

    Node* _head = nullptr;

public:
    // Using's :
    using value_type = Key;
    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;

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
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;  
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    // Hash policy :
    void rehash(std::size_t new_buffer_size);
	void reserve(std::size_t new_buffer_size);
};

}
}

#include "../../src/Associative/HashSet.tpp"