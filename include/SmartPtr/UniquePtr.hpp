#pragma once

#include <memory>

#include "../../include/Optional/Optional.hpp"



namespace tmn_smart_ptr {

//                                      GENERAL REALIZATION :

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
private:
// Fields :
    T* ptr;
    [[no_unique_address]] Deleter deleter;

public:
// Using's :
    using Pointer = T*;

public:
// Constructors & assignment & conversion & destructors :
    UniquePtr() noexcept;
    explicit UniquePtr(Pointer ptr) noexcept;

    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    template <typename U, typename E>
    UniquePtr(UniquePtr<U, E>&& rhs) noexcept;

    template <typename U, typename E>
    UniquePtr& operator=(UniquePtr<U, E>&& rhs) noexcept;
    
    ~UniquePtr();

// Modifiers :
    Pointer release() noexcept;
    void reset(Pointer p = Pointer()) noexcept;
    void swap(UniquePtr<T, Deleter>& other) noexcept;

// Observers :
    const Pointer get() const noexcept;
    explicit operator bool() const noexcept;
    tmn::Optional<T> operator*() const;
    Pointer operator->() const noexcept;
};



//                                      SPECIALIZATION TEMPLATE :

template <typename T>
class UniquePtr<T[], std::default_delete<T[]>> { 
private:
// Fields :
    T* ptr;
    [[no_unique_address]] std::default_delete<T[]> deleter;

public:
// Constructors & assignment & conversion & destructors :
    UniquePtr() noexcept;
    template <typename U>
    explicit UniquePtr(U* ptr) noexcept;

    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    template <typename U, typename E>
    UniquePtr(UniquePtr<U, E>&& rhs) noexcept;

    template <typename U, typename E>
    UniquePtr& operator=(UniquePtr<U, E>&& rhs) noexcept;
    
    ~UniquePtr();

// Modifiers :
    T* release() noexcept;

    template <typename U>
    void reset(U p) noexcept;
    
    void swap(UniquePtr<T[], std::default_delete<T[]>>& other) noexcept;

// Observers :
    const T* get() const noexcept;
    explicit operator bool() const noexcept;
    tmn::Optional<T> operator[](std::size_t i) const;
};

}



#include "../../src/SmartPtr/UniquePtr.tpp"