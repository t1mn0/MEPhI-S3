#include "../../include/SmartPtr/UniquePtr.hpp"
#include "../../include/Exceptions/Exception.hpp"



namespace tmn {
namespace smart_ptr {

//                                      GENERAL REALIZATION :

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion & destructors :
template <typename T, typename Deleter>
UniquePtr<T, Deleter>::UniquePtr() noexcept : ptr(nullptr) {}
    
template <typename T, typename Deleter>
UniquePtr<T, Deleter>::UniquePtr(Pointer ptr) noexcept : ptr(ptr), deleter(Deleter()) {}

template <typename T, typename Deleter>
template <typename U, typename E>
UniquePtr<T, Deleter>::UniquePtr(UniquePtr<U, E>&& rhs) noexcept 
    : ptr(rhs.ptr), deleter(std::move(rhs.deleter)) {
    rhs.ptr = nullptr;
}

template <typename T, typename Deleter>
template <typename U, typename E>
UniquePtr<T, Deleter>& UniquePtr<T, Deleter>::operator=(UniquePtr<U, E>&& rhs) noexcept {
    ptr = rhs.ptr;
    deleter = std::move(rhs.deleter);
    rhs.ptr = nullptr;
    return *this;
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::~UniquePtr() {
    deleter(ptr);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Modifiers :

template <typename T, typename Deleter>
auto UniquePtr<T, Deleter>::release() noexcept 
    -> UniquePtr<T, Deleter>::Pointer 
{
    Pointer tmp = ptr;
    ptr = nullptr;
    return tmp;
}

template <typename T, typename Deleter>
void UniquePtr<T, Deleter>::reset(Pointer p) noexcept {
    deleter(ptr);
    ptr = p;
}

template <typename T, typename Deleter>
void UniquePtr<T, Deleter>::swap(UniquePtr<T, Deleter>& rhs) noexcept {
    std::swap(ptr, rhs.ptr);
    std::swap(deleter, rhs.deleter);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Observers :

template <typename T, typename Deleter>
typename UniquePtr<T, Deleter>::Pointer  UniquePtr<T, Deleter>::get() const noexcept {
    return ptr;
}

template <typename T, typename Deleter>
UniquePtr<T, Deleter>::operator bool() const noexcept {
    return ptr != nullptr;
}

template <typename T, typename Deleter>
tmn::Optional<T> UniquePtr<T, Deleter>::operator*() const {
    if (ptr){
        return tmn::Optional<T>(*ptr);
    }
    else{
        return tmn::Optional<T>();
    }
}

template <typename T, typename Deleter>
auto UniquePtr<T, Deleter>::operator->() const noexcept
    -> UniquePtr<T, Deleter>::Pointer 
{
    return ptr;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 



//                                      SPECIALIZATION TEMPLATE :

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion & destructors :
template <typename T>
UniquePtr<T[], std::default_delete<T[]>>::UniquePtr() noexcept : ptr(nullptr) {}
    
template <typename T>
template <typename U>
UniquePtr<T[], std::default_delete<T[]>>::UniquePtr(U* ptr) noexcept : ptr(static_cast<T*>(ptr)) {}

template <typename T>
template <typename U, typename E>
UniquePtr<T[], std::default_delete<T[]>>::UniquePtr(UniquePtr<U, E>&& rhs) noexcept 
    : ptr(rhs.ptr), deleter(std::move(rhs.deleter)) {
    rhs.ptr = nullptr;
}

template <typename T>
template <typename U, typename E>
auto UniquePtr<T[], std::default_delete<T[]>>::operator=(UniquePtr<U, E>&& rhs) noexcept 
    -> UniquePtr<T[], std::default_delete<T[]>>&
{
    ptr = rhs.ptr;
    deleter = std::move(rhs.deleter);
    rhs.ptr = nullptr;
    return *this;
}

template <typename T>
UniquePtr<T[], std::default_delete<T[]>>::~UniquePtr() {
    deleter(ptr);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Modifiers :

template <typename T>
T* UniquePtr<T[], std::default_delete<T[]>>::release() noexcept {
    T* tmp = ptr;
    ptr = nullptr;
    return tmp;
}

template <typename T>
template <typename U>
void UniquePtr<T[], std::default_delete<T[]>>::reset(U p) noexcept {
    T* tmp = static_cast<T*>(p);
    deleter(ptr);
    ptr = tmp;
}

template <typename T>
void UniquePtr<T[], std::default_delete<T[]>>::swap(UniquePtr<T[], std::default_delete<T[]>>& rhs) noexcept {
    std::swap(ptr, rhs.ptr);
    std::swap(deleter, rhs.deleter);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Observers :

template <typename T>
const T* UniquePtr<T[], std::default_delete<T[]>>::get() const noexcept {
    return ptr;
}

template <typename T>
UniquePtr<T[], std::default_delete<T[]>>::operator bool() const noexcept {
    return ptr != nullptr;
}

template <typename T>
tmn::Optional<T> UniquePtr<T[], std::default_delete<T[]>>::operator[](std::size_t i) const {
    if (ptr[i]){
        return tmn::Optional<T>(ptr[i]);
    }
    else{
        return tmn::Optional<T>();
    }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// MakeUnique :

template <typename T, typename... Args>
UniquePtr<T> make_unique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
UniquePtr<T> make_unique(std::size_t size) {
    if (size == 0){
        throw tmn::exception::Exception("Bad size argument");
    }
    T* ptr = new T[size];
    return UniquePtr<T[], std::default_delete<T[]>()>(ptr);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

}
}