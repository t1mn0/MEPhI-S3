#pragma once

#include <cstddef>

#include "../Optional/Optional.hpp"



namespace tmn_smart_ptr {

template <typename T>
class SharedPtr {
private:
// Fields :
    T* ptr;
    size_t* counter;

public:
// Constructors & assignment & conversion & destructors :
    SharedPtr();
    explicit SharedPtr(T* ptr);

    SharedPtr(const SharedPtr& rhs) noexcept;
    SharedPtr& operator=(const SharedPtr& rhs) noexcept;

    SharedPtr(SharedPtr&& rhs) noexcept;
    SharedPtr& operator=(SharedPtr&& rhs) noexcept;

    ~SharedPtr();

// Modifiers :
    void reset(T* p = nullptr);
    void swap(SharedPtr<T>& rhs) noexcept;

// Observers :
    const T* get() const noexcept;
    explicit operator bool() const noexcept;
    tmn::Optional<T> operator*() const;
    T* operator->() const noexcept;

    std::size_t use_count() const noexcept;
    bool unique() const noexcept;
};

}



#include "../../src/SmartPtr/SharedPtr.tpp"