#include <memory>

#include "../../include/SmartPtr/SharedPtr.hpp"
#include "../../include/Exceptions/Exception.hpp"



namespace tmn_smart_ptr {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion & destructors :

template <typename T>
SharedPtr<T>::SharedPtr() : ptr(nullptr), counter(new size_t(0)) { }

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) : ptr(ptr), counter(new size_t(1)) { }

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& rhs) noexcept: ptr(rhs.ptr), counter(rhs.counter) {
    if (counter != nullptr && ptr != nullptr) {
        ++(*counter);
    }
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& rhs) noexcept {
    if (this != &rhs) {
        --(*counter);
        if (*counter == 0) {
            delete ptr;
            delete counter;
        }

        ptr = rhs.ptr;
        counter = rhs.counter;
        ++(*counter);
    }

    return *this;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& rhs) noexcept : ptr(rhs.ptr), counter(rhs.counter) {
    rhs.ptr = nullptr;
    rhs.counter = nullptr;
}

template <typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& rhs) noexcept {
    if (this != &rhs) {
        --(*counter);
        if (*counter == 0) {
            delete ptr;
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
        --(*counter);
        if (counter == 0) {
            delete ptr;
            delete counter;
        }

        ptr = nullptr;
        counter = nullptr;
    }

    if (!ptr && counter){
        delete counter;
    }

    if (ptr && !counter){
        ptr = nullptr;
    }
    
    if (rhs) {
        ptr = rhs;
        counter = new size_t(0);
        if(counter){
            ++(*counter);
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
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Observers :

template <typename T>
const T* SharedPtr<T>::get() const noexcept {
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
        return *counter;
    }
    return 0;
}

template <typename T>
bool SharedPtr<T>::unique() const noexcept {
    if (ptr){
        return *counter == 1;
    }
    return false;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

}