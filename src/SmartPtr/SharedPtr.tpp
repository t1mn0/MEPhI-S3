#include <memory>

#include "../../include/SmartPtr/SharedPtr.hpp"
#include "../../include/Exceptions/Exception.hpp"

namespace tmn_smart_ptr {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion & destructors :

template <typename T>
SharedPtr<T>::SharedPtr(ControlBlockForMakeShared<T>* control_block) : ptr(&control_block->value), counter(control_block) {
    ++counter->strong_count;
}

template <typename T>
SharedPtr<T>::SharedPtr() : ptr(nullptr), counter(new BaseControlBlock()) { }

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) : ptr(ptr), counter(new BaseControlBlock()) {
    ++counter->strong_count;
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& rhs) noexcept: ptr(rhs.ptr), counter(rhs.counter) {
    if (counter != nullptr) {
        ++counter->strong_count;
    }
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& rhs) noexcept {
    if (this != &rhs) {
        --counter->strong_count;
        if (counter->strong_count == 0) {
            delete counter;
        }

        ptr = rhs.ptr;
        counter = rhs.counter;
        ++counter->strong_count;
    }

    return *this;
}

template <typename T>
template <typename U>
SharedPtr<T>::SharedPtr(const SharedPtr<U>& rhs, T* ptr) noexcept : ptr(ptr), counter(rhs.counter) {
    if (counter){
        ++counter->strong_count;
    }
    else {
        throw tmn_exception::Exception("Bad Control Block");
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& rhs) noexcept : ptr(rhs.ptr), counter(rhs.counter) {
    rhs.ptr = nullptr;
    rhs.counter = nullptr;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& rhs) noexcept {
    if (this != &rhs) {
        --counter->strong_count;
        if (counter->strong_count == 0) {
            delete counter;
        }

        ptr = rhs.ptr;
        counter = rhs.counter;
        rhs.ptr = nullptr;
        rhs.counter = nullptr;
    }
    return *this;
}

template <typename T>
SharedPtr<T>::~SharedPtr() {
    reset();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Modifiers :

template <typename T>
void SharedPtr<T>::reset(T* rhs) {
    if (ptr) {
        --counter->strong_count;
        if (counter->strong_count == 0) {
            delete ptr;
            delete counter;
        }

        ptr = nullptr;
        counter = nullptr;
    }
    
    if (rhs) {
        ptr = rhs;
        counter = new BaseControlBlock();
        if(counter){
            ++counter->strong_count;
        }
        else{
            throw tmn_exception::Exception("Bad Control Block");
        }
    }
}

template <typename T>
void SharedPtr<T>::swap(SharedPtr<T>& rhs) noexcept {
    std::swap(this->ptr, rhs.ptr);
    std::swap(this->counter, rhs.counter);
    // SharedPtr<T> tmp = std::move(*this);
    // *this = std::move(rhs);
    // rhs = std::move(tmp);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Observers :

template <typename T>
T* SharedPtr<T>::get() const noexcept {
    return ptr;
}

template <typename T>
SharedPtr<T>::operator bool() const noexcept {
    return ptr != nullptr;
}

template <typename T>
tmn::Optional<T> SharedPtr<T>::operator*() const {
    if (ptr){
        return tmn::Optional<T>(*ptr);
    }
    else{
        return tmn::Optional<T>();
    }
}
    
template <typename T>
T* SharedPtr<T>::operator->() const noexcept {
    return ptr;
}

template <typename T>
std::size_t SharedPtr<T>::use_count() const noexcept {
    if (ptr) {
        return counter->strong_count;
    }
    return 0;
}

template <typename T>
bool SharedPtr<T>::unique() const noexcept {
    if (ptr){
        return counter->strong_count == 1;
    }
    return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 



// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// MakeShared :

template <typename T, typename... Args>
SharedPtr<T>* MakeShared(Args&&... args) {
    auto* cb = new typename SharedPtr<T>::template ControlBlockForMakeShared<T>{T(std::forward<Args>(args)...)};
    SharedPtr<T>* sp = new SharedPtr<T>(cb);

    return sp;
}

}