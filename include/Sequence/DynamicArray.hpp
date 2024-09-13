#pragma once

#include <cstdint>
#include <memory>
#include <initializer_list>

#include "../Iterator/Iterator.hpp"
#include "../../include/tmn.hpp"



namespace tmn_sequence {

template <typename T, class Allocator = std::allocator<T>>
class DynamicArray {
private:
    T* array = nullptr;
    std::size_t _size = 0;
    std::size_t _capacity = 0;
    Allocator _alloc;

    template <bool isConst>
    struct common_iterator {
    public:
        using conditional_ptr = tmn::conditional_t<isConst, const T*, T*>;
        using conditional_ref = tmn::conditional_t<isConst, const T&, T&>;
        using iterator_category	= tmn_iterator::contiguous_iterator_tag;
        using value_type = T;
        using difference_type = std::size_t;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
    
    private:
        conditional_ptr ptr;

    public:
        common_iterator() : ptr(nullptr) {}
        explicit common_iterator(const pointer ptr) : ptr(ptr) {}
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
        common_iterator<isConst> operator+(difference_type n) const;
        common_iterator<isConst> operator-(difference_type n) const;
        difference_type operator-(const common_iterator<isConst>& other) const;
        common_iterator<isConst>& operator+=(difference_type n);
        common_iterator<isConst>& operator-=(difference_type n);

        bool operator<(const common_iterator<isConst>& other) const;
        bool operator<=(const common_iterator<isConst>& other) const;
        bool operator>(const common_iterator<isConst>& other) const;
        bool operator>=(const common_iterator<isConst>& other) const;
    };

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
    DynamicArray();
    explicit DynamicArray(std::size_t _size);
    explicit DynamicArray(std::size_t _size, const T& default_element);
    DynamicArray(const DynamicArray<T, Allocator>& other);
    DynamicArray(DynamicArray<T, Allocator>&& other) noexcept;
    DynamicArray(std::initializer_list<T> lst);

    void swap(DynamicArray<T, Allocator>& other);
    DynamicArray<T, Allocator>& operator=(const DynamicArray<T, Allocator>& other);

    ~DynamicArray();

    // Capacity & size :
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    bool empty() const noexcept;

    void reserve(std::size_t new_capacity);
    void resize(std::size_t new_size, const T& value = T());
    DynamicArray<T, Allocator>& clear();

    DynamicArray<T, Allocator>& push_back(const T& value);
    DynamicArray<T, Allocator>& push_back(T&& value);
    DynamicArray<T, Allocator>& pop_back();

    template <typename... Args>
    void emplace_back(Args... args);

    // Element access methods :
    const T& front() const;
    T& front();
    const T& back() const;
    T& back();
    DynamicArray<T, Allocator>& set(std::size_t index, const T& item);
    T& get(std::size_t index);
    const T& get(std::size_t index) const;
    T& operator[](std::size_t index) noexcept;
    const T& operator[](std::size_t index) const noexcept;
    const T* data() const noexcept;
    T* data() noexcept;

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

};

}



#include "../../src/Sequence/DynamicArray.cpp"