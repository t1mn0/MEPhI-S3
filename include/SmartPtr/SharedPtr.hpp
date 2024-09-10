#pragma once

#include <cstddef>
#include "../Optional/Optional.hpp"



namespace tmn_smrt_ptr {



template <typename T>
struct SharedPtr {
private:
// Supporting structures & constructor & friend:
    struct BaseControlBlock {
        std::size_t strong_count = 0;
        virtual ~BaseControlBlock() = default;
    };

    // ControlBlock является шаблонными для наследования
    template <typename U>
    struct ControlBlockForMakeShared : BaseControlBlock {
        U value;
    };

    SharedPtr(BaseControlBlock* control_block);

    template <typename U, typename... Args>
    friend SharedPtr<U> MakeShared(Args&&...);

private:
// Fields :
    T* ptr;
    BaseControlBlock* counter;

public:
// Constructors & assignment & conversion & destructors :
    SharedPtr();
    explicit SharedPtr(T* ptr);

    SharedPtr(const SharedPtr& rhs) noexcept;
    SharedPtr& operator=(const SharedPtr& rhs) noexcept;
    
    // Для наследования (кастов)
    template <typename U>
    SharedPtr(const SharedPtr<U>& rhs, T* ptr) noexcept;

    SharedPtr(SharedPtr&& rhs) noexcept;
    SharedPtr& operator=(SharedPtr&& rhs) noexcept;

    ~SharedPtr();

// Modifiers :
    void reset(T* p = nullptr);
    void swap(SharedPtr<T>& rhs) noexcept;

// Observers :
    T* get() const noexcept;
    explicit operator bool() const noexcept;
    tmn::Optional<T>& operator*() const;
    T* operator->() const noexcept;

    std::size_t use_count() const noexcept;
    bool unique() const noexcept;
};



// MakeShared :
template <typename T, typename... Args>
SharedPtr<T> MakeShared(Args&&... args);



// pointer_cast's :
template<class T, class U>
SharedPtr<T> static_pointer_cast(const SharedPtr<U>& r) noexcept;

template<class T, class U>
SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U>& r) noexcept;

template<class T, class U>
SharedPtr<T> const_pointer_cast(const SharedPtr<U>& r) noexcept;

template<class T, class U>
SharedPtr<T> reinterpret_pointer_cast(const SharedPtr<U>& r) noexcept;



}



#include "../../src/SmartPtr/SharedPtr.cpp"