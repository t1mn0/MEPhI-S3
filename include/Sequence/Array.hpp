#pragma once

#include <cstdint>
#include <memory>
#include <initializer_list>

#include "../Iterator/Iterator.hpp"
#include "../../include/tmn.hpp"
#include "Sequence.hpp"

namespace tmn {
namespace sequence {

template <typename T, std::size_t N>
class Array /* : public Sequence */ {
private:
    T* array = nullptr;

    template <bool isConst>
    struct common_iterator{
    public:
        using conditional_ptr = tmn::conditional_t<isConst, const T*, T*>;
        using conditional_ref = tmn::conditional_t<isConst, const T&, T&>;
        using iterator_category	= tmn::iterator::contiguous_iterator_tag;
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
    using reverse_iterator = tmn::iterator::reverse_iterator<iterator>;
    using const_reverse_iterator = tmn::iterator::reverse_iterator<const_iterator>;

    // Constructors & assignment & conversion :
    Array();
    explicit Array(const T& default_element);
    Array(const Array<T, N>& other);
    Array(std::initializer_list<T> lst);

    void swap(Array<T, N>& other);
    Array<T, N>& operator=(const Array<T, N>& other);

    ~Array();

    // Capacity & size :
    std::size_t size() const noexcept override;
    bool empty() const noexcept override;
    
    void fill(const T& value);

    // Element access methods :
    const T& front() const;
    T& front();
    const T& back() const;
    T& back();
    Array<T, N>& set(std::size_t index, const T& item);
    T& get(std::size_t index) override;
    const T& get(std::size_t index) const override;
    T& operator[](std::size_t index) noexcept override;
    const T& operator[](std::size_t index) const noexcept override; 
    const T* data() const noexcept;
    T* data() noexcept;

    // Iterator methods :
    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;  
    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept; 

};

}
}



#include "../../src/Sequence/Array.tpp"