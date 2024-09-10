#pragma once

#include "../../Optional/Optional.hpp"

#include <memory>



namespace tmn_smrt_ptr {



//                                      GENERAL REALIZATION :

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
private:
// Fields :
    T* ptr;
    [[no_unique_address]] Deleter deleter;

public:
// Using's :
    using pointer = T*;
    // using pointer = typename std::conditional<has_pointer<Deleter>, std::remove_reference_t<Deleter>::pointer, T*>;

public:
// Constructors & assignment & conversion & destructors :
    UniquePtr() noexcept;
    explicit UniquePtr(pointer ptr) noexcept;

    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    template <typename U, typename E>
    UniquePtr(UniquePtr<U, E>&& rhs) noexcept;

    template <typename U, typename E>
    UniquePtr& operator=(UniquePtr<U, E>&& rhs) noexcept;
    
    ~UniquePtr();

// Modifiers :
    pointer release() noexcept;
    void reset(pointer p = pointer()) noexcept;
    void swap(UniquePtr<T, Deleter>& other) noexcept;

// Observers :
    pointer get() const noexcept;
    explicit operator bool() const noexcept;
    tmn::Optional<T>& operator*() const;
    pointer operator->() const noexcept;
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
    explicit UniquePtr(U ptr) noexcept;

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
    T* get() const noexcept;
    explicit operator bool() const noexcept;
    tmn::Optional<T>& operator[](std::size_t i) const;
};


// MakeUnique :
template <typename T, typename... Args>
UniquePtr<T> MakeUnique(Args&&... args);

template <typename T>
UniquePtr<T> MakeUnique(std::size_t size);



// Output to stream :
template <class CharT, class Traits, class T, class D>
auto operator<<(std::basic_ostream<CharT, Traits>& os, const UniquePtr<T, D>& p)
    -> std::basic_ostream<CharT, Traits>&;



}