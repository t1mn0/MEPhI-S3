#include "../../include/SmartPtr/SharedPtr.hpp"
#include "../../include/Exceptions/Exception.hpp"

#include <memory>



namespace tmn_smrt_ptr {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion & destructors :

template <typename T>
SharedPtr<T>::SharedPtr(BaseControlBlock* control_block) : ptr(nullptr), counter(control_block) {
    if (counter != nullptr) {
        ++counter->strong_count;

        if (auto* cblock = dynamic_cast<ControlBlockForMakeShared<T>*>(counter)) {
            ptr = &cblock->value; 
        }
    }
    else{
        throw tmn_exception::Exception("Bad Control Block");
    }
}

template <typename T>
SharedPtr<T>::SharedPtr() : ptr(nullptr), counter(new BaseControlBlock()) {
    if (counter == nullptr) {
        throw tmn_exception::Exception("Bad Control Block");
    }
}

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr) : ptr(ptr), counter(new BaseControlBlock()) {
    ++counter->strong_count;
    if (counter == nullptr) {
        throw tmn_exception::Exception("Bad Control Block");
    }
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
SharedPtr<T>::SharedPtr(const SharedPtr<U>& rhs, T* ptr) noexcept
    : ptr(ptr), counter(rhs.counter) {
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

// template <typename T, typename... Args>
// SharedPtr<T> MakeShared(Args&&... args) {
//     auto* p = new typename SharedPtr<T>::template ControlBlockForMakeShared(T(std::forward<Args>(args)...));
//     return SharedPtr<T>(p);
// }

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Output to stream :

template <class CharT, class Traits, class T>
auto operator<<(std::basic_ostream<CharT, Traits> &os, const SharedPtr<T>& p) 
    -> std::basic_ostream<CharT, Traits> &
{
    if (p) {
        os << *p;
    } 
    else {
        os << "nullptr";
    }
    
    return os;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// pointer_cast's :

template<class T, class U>
SharedPtr<T> static_pointer_cast(const SharedPtr<U>& r) noexcept {
    auto p = static_cast<T*>(r.get());
    return SharedPtr<T>{r, p};
}

template<class T, class U>
SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U>& r) noexcept {
    if (auto p = dynamic_cast<T*>(r.get())) {
        return SharedPtr<T>{r, p};
    }
    else {
        return SharedPtr<T>{};
    }
}

template<class T, class U>
SharedPtr<T> const_pointer_cast(const SharedPtr<U>& r) noexcept {
    auto p = const_cast<T*>(r.get());
    return SharedPtr<T>{r, p};
}

template<class T, class U>
SharedPtr<T> reinterpret_pointer_cast(const SharedPtr<U>& r) noexcept {
    auto p = reinterpret_cast<T*>(r.get());
    return SharedPtr<T>{r, p};
}

}