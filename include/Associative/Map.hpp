#pragma once

#include <cstdint>
#include <memory>
#include <initializer_list>

#include "../Iterator/Iterator.hpp"
#include "../Pair/Pair.hpp"
#include "../Optional/Optional.hpp"
#include "../Sequence/ArraySequence.hpp"
#include "Hash.hpp"
#include "../tmn.hpp"

namespace tmn_associative {

template <class Key, class Value>
class Map {
private:
    // Support structures and methods:
    struct RBTreeNode {
        tmn::Pair<const Key, Value> pair;

        RBTreeNode* parent = nullptr; 
        RBTreeNode* left = nullptr;
        RBTreeNode* right = nullptr;

        bool is_red = false;

        // TODO : прошивка дерева для итерации 
        // bool left_is_thread = false;
        // bool right_is_thread = false;

        RBTreeNode(const tmn::Pair<const Key, Value>& other_pair) noexcept;
    };

    template <bool isConst>
    struct common_iterator {
    public:
        using conditional_layer_ptr = tmn::conditional_t<isConst, const RBTreeNode*, RBTreeNode*>;
        using conditional_ptr = tmn::conditional_t<isConst, const tmn::Pair<const Key, Value>*, tmn::Pair<const Key, Value>*>;
        using conditional_ref = tmn::conditional_t<isConst, const tmn::Pair<const Key, Value>&, tmn::Pair<const Key, Value>&>;
        
        // using iterator_category	= tmn_iterator:: ? ? ? ;
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
    };

    RBTreeNode* leftmost(RBTreeNode* node); 
    RBTreeNode* rightmost(RBTreeNode* node);
    
    void rotate_left(RBTreeNode* pivot_node); 
    void rotate_right(RBTreeNode* pivot_node); 

    void inner_insert(RBTreeNode* new_node);
    void inner_erase(RBTreeNode* erase);

    void transplant(RBTreeNode* u, RBTreeNode* v); // <swap>

    void clear_tree(RBTreeNode* node);

    RBTreeNode* find_node(const Key& key);

private:
    // Fields :
    RBTreeNode* _root = nullptr;
    std::size_t _size = 0;
    std::size_t _black_height = 0;
    std::allocator<RBTreeNode> _alloc_node;


public:
    // Using's :
    using key_type = Key;
    using mapped_type = Value;
    using value_type = tmn::Pair<const Key, Value>;
    // using iterator = common_iterator<false>;
    // using const_iterator = common_iterator<true>;
    using allocator_traits_node = std::allocator_traits<std::allocator<RBTreeNode>>;


    // Constructors & assignment & conversion :
    Map() noexcept = default;
    //Map(const Map<Key, Value>& other);
    Map(Map<Key, Value>&& other);
    Map(std::initializer_list<tmn::Pair<const Key, Value>> lst);

    void swap(Map<Key, Value>& other);
    // Map& operator=(const Map<Key, Value>& other);
    Map& operator=(Map&& other) noexcept;

    ~Map();

    // Capacity & size & get fields:
    std::size_t size() const noexcept;
    bool empty() const noexcept;

    // Modifiers :
    bool insert(const tmn::Pair<const Key, Value>& value);
    // bool insert(tmn::Pair<const Key, Value>&& value);

    // template <typename InputIterator>
    // void insert(InputIterator b, InputIterator e);

    bool erase(const Key& key);
    //bool erase(Key&& key);

    void clear();

    // Element access methods :
    // tmn::Optional<Value> get(const Key& key) const noexcept;
    // tmn::Optional<Value> get(Key&& key) const noexcept;

    //const Value& operator[](const Key& key) const;
    // Value& operator[](const Key& key);

    // bool contains(const Key& key) const;

    // tmn_sequence::ArraySequence<Key> keys() const;
    // tmn_sequence::ArraySequence<Value> values() const;

    // Iterator methods :
    // iterator begin() noexcept;
    // const_iterator begin() const noexcept;
    // const_iterator cbegin() const noexcept;  
    // iterator end() noexcept;
    // const_iterator end() const noexcept;
    // const_iterator cend() const noexcept;
};

}

#include "../../src/Associative/Map.tpp"