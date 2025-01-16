#pragma once

#include <memory>

#include "../Optional/Optional.hpp"

namespace tmn {
namespace smart_ptr {

template <typename T, typename Deleter = std::default_delete<T>>
class UniquePtr {
private:
// Fields :
    T* ptr;
    [[no_unique_address]] Deleter deleter;

    template <typename U, typename E>
    friend class UniquePtr;

public:
// Constructors & assignment & conversion & destructors :
    UniquePtr() noexcept;
    UniquePtr(T* ptr) noexcept;

    template <typename U>
    explicit UniquePtr(U* ptr) noexcept;

    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    UniquePtr(UniquePtr<T, Deleter>&& rhs) noexcept;

    UniquePtr& operator=(UniquePtr<T, Deleter>&& rhs) noexcept;

    ~UniquePtr();

// Modifiers :
    T* release() noexcept;
    void reset(T* p) noexcept;
    void swap(UniquePtr<T, Deleter>& other) noexcept;

// Observers :
    T* get() const noexcept;
    explicit operator bool() const noexcept;
    tmn::Optional<T> operator*() const;
    T* operator->() const noexcept;
};

template <typename T>
class UniquePtr<T[], std::default_delete<T[]>> {
private:
// Fields :
    T* ptr;
    [[no_unique_address]] std::default_delete<T[]> deleter;

public:
// Constructors & assignment & conversion & destructors :
    UniquePtr() noexcept;
    UniquePtr(T* ptr) noexcept;

    UniquePtr(const UniquePtr& other) = delete;
    UniquePtr& operator=(const UniquePtr& other) = delete;

    UniquePtr(UniquePtr<T[]>&& rhs) noexcept;
    UniquePtr<T[]>& operator=(UniquePtr<T[]>&& rhs) noexcept;

    ~UniquePtr();

// Modifiers :
    T* release() noexcept;

    void reset(T* p) noexcept;

    void swap(UniquePtr<T[], std::default_delete<T[]>>& other) noexcept;

// Observers :
    T* get() const noexcept;
    explicit operator bool() const noexcept;
    tmn::Optional<T> operator[](std::size_t i) const;
};

}
}

#include "../../src/SmartPtr/UniquePtr.tpp"
