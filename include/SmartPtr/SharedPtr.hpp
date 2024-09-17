#pragma once

#include <cstddef>
#include <type_traits>

#include "../Optional/Optional.hpp"

namespace tmn_smart_ptr {

// Forward declaration of SharedPtr
// template <typename T>
// struct SharedPtr;

// Concept to check if a type is not a specialization of SharedPtr
// template <typename T>
// concept IsNotSharedPtr = !std::is_same_v<SharedPtr<std::remove_cvref_t<T>>, T>;

// Concept to check if a type is derived from or the same as another type
// template <typename Derived, typename Base>
// concept IsDerivedOrSame = std::is_base_of_v<Base, Derived> || std::is_same_v<Base, Derived>;



template <typename T>
class SharedPtr {
private:
// Supporting structures & constructor & friend:
    struct BaseControlBlock {
        std::size_t strong_count = 0;
        virtual ~BaseControlBlock() = default;
    };

    // ControlBlock является шаблонными для наследования
    template <typename U>
    struct ControlBlockForMakeShared : BaseControlBlock {
        template <typename... Args>
        ControlBlockForMakeShared(Args&&... args) : value(std::forward<Args>(args)...) {}
        U value;
    };

    SharedPtr(ControlBlockForMakeShared<T>* control_block);

    template <typename U, typename... Args>
    friend SharedPtr<U>* MakeShared(Args&&...);

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
    tmn::Optional<T> operator*() const;
    T* operator->() const noexcept;

    std::size_t use_count() const noexcept;
    bool unique() const noexcept;
};



// MakeShared :
template <typename T, typename... Args>
SharedPtr<T>* MakeShared(Args&&... args);


}

#include "../../src/SmartPtr/SharedPtr.tpp"