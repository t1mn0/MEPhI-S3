#pragma once

#include <cstdint>
#include <memory>
#include <initializer_list>

#include "../Iterator/Iterator.hpp"
#include "../../include/tmn.hpp"



namespace tmn_sequence{

template <typename T, class Allocator = std::allocator<T>>
class List{
private:
    // Support structures :
    struct Node {
		T value;
		Node* next;
		Node* prev;
		Node(const T& value) : value(value), next(this), prev(this) {};
		Node(const T& value, Node* next, Node* prev) : value(value), next(next), prev(prev) {};

        friend std::ostream& operator<<(std::ostream& out, const Node* node) {
            out << node->value;
            return out;
        }
	};

    template <bool isConst>
    struct common_iterator{
    public:
        template <typename U, class Allocator2> 
        friend class List;
        
        using conditional_layer_ptr = tmn::conditional_t<isConst, const Node*, Node*>;
        using conditional_ptr = tmn::conditional_t<isConst, const T*, T*>;
        using conditional_ref = tmn::conditional_t<isConst, const T&, T&>;
        using iterator_category	= tmn_iterator::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::size_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
    
    private:
        conditional_layer_ptr ptr;

    public:
        common_iterator() : ptr(nullptr) {}
        explicit common_iterator(conditional_layer_ptr ptr) : ptr(ptr) {}
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

private:
    // Fields :
    std::size_t _size = 0;
    Allocator _alloc;
    Node* dummy_node;

public:
    // Using's :
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using iterator = common_iterator<false>;
    using const_iterator = common_iterator<true>;
    using reverse_iterator = tmn_iterator::reverse_iterator<iterator>;
    using const_reverse_iterator = tmn_iterator::reverse_iterator<const_iterator>;
    using allocator_traits = std::allocator_traits<Allocator>;

    // Constructors & assignment & conversion :
    List() : dummy_node(new Node(T())) {}
    explicit List(std::size_t _size);
    explicit List(std::size_t _size, const T& default_element);
    List(const List<T, Allocator>& other);
    List(std::initializer_list<T> lst);

    void swap(List<T, Allocator>& other);
    List<T, Allocator>& operator=(const List<T, Allocator>& other);

    ~List();

    // Capacity & size :
    std::size_t size() const noexcept;
    bool empty() const noexcept;

    void resize(std::size_t new_size, const T& value = T());
    List<T, Allocator>& clear();

    List<T, Allocator>& push_front(const T& value);
    List<T, Allocator>& push_back(const T& value);
    List<T, Allocator>& pop_front();
    List<T, Allocator>& pop_back();

    // Element access methods :
    const T& front() const;
    T& front();
    const T& back() const;
    T& back();
    List<T, Allocator>& set(std::size_t index, const T& item);
    T& get(std::size_t index);
    const T& get(std::size_t index) const;
    T& operator[](std::size_t index);
    const T& operator[](std::size_t index) const;

    // Iterator methods :
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;  
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;  
    tmn_iterator::reverse_iterator<iterator> rbegin() noexcept;
    tmn_iterator::reverse_iterator<iterator> rend() noexcept;

    iterator insert(iterator pos, const T& value);
    iterator insert(iterator pos, size_type count, const T& value);
    iterator erase(iterator pos);

    // Other modifiers :
    void merge(List<T, Allocator>& other);
};

}

#include "../../src/Sequence/List.tpp"