#include <utility>
#include <cstdint>

#include "../../include/Function/Function.hpp"

namespace tmn {

template <typename ReturnType, typename... Args>
template <typename F>
ReturnType Function<ReturnType (Args...)>::FunctionWrapper<F>::call(Args&&... args) {
    return f_object(std::forward<Args>(args)...);
}

template <typename ReturnType, typename... Args>
template <typename F>
void Function<ReturnType (Args...)>::FunctionWrapper<F>::move_by_ptr(void* to) {
    new (to) FunctionWrapper<F>(std::move(f_object));
}

template <typename ReturnType, typename... Args>
template <typename F>
void Function<ReturnType(Args...)>::FunctionWrapper<F>::small_copy(void* to) const {
    new (to) FunctionWrapper<F>(f_object);
}

template <typename ReturnType, typename... Args>
template <typename F>
smart_ptr::UniquePtr<typename Function<ReturnType(Args...)>::Wrapper> Function<ReturnType(Args...)>::FunctionWrapper<F>::full_copy() const{
    //return smart_ptr::make_unique<FunctionWrapper<F>>(f_object);
    Wrapper* ptr = new FunctionWrapper<F>(f_object);
    return smart_ptr::UniquePtr<Wrapper>(ptr);
}

template <typename ReturnType, typename... Args>
void Function<ReturnType (Args...)>::clear() {
    if (is_object_small){
        reinterpret_cast<Wrapper*>(buffer)->~Wrapper();
    }
    else{
        big_object_ptr.~UniquePtr();
    }
}

template <typename ReturnType, typename... Args>
Function<ReturnType (Args...)>::operator bool() const noexcept {
    return is_object_small || static_cast<bool>(big_object_ptr);
}

template <typename ReturnType, typename... Args>
Function<ReturnType (Args...)>::Function(const Function& other_function) {
    if (other_function.is_object_small){
        reinterpret_cast<const Wrapper*>(other_function.buffer)->small_copy(buffer);
    }
    else {
        new (buffer) smart_ptr::UniquePtr((other_function.big_object_ptr).operator->()->full_copy());
    }
    is_object_small = other_function.is_object_small;
}

template <typename ReturnType, typename... Args>
Function<ReturnType (Args...)>::Function(Function&& other_function){
    if (other_function.is_object_small) {
        reinterpret_cast<Wrapper*>(other_function.buffer)->move_by_ptr(buffer);
        new (other_function.buffer) smart_ptr::UniquePtr<Wrapper>(nullptr);
    }
    else {
        new (buffer) smart_ptr::UniquePtr(std::move(other_function.big_object_ptr));
    }
    is_object_small = other_function.is_object_small;
    other_function.is_object_small = false;
}

template <typename ReturnType, typename... Args>
template <typename F>
Function<ReturnType (Args...)>::Function(F f){
    if constexpr (std::is_nothrow_constructible<F>::value && sizeof(f) > BUFFER_SIZE) {
        is_object_small = false;
        new (buffer) smart_ptr::UniquePtr<FunctionWrapper<F>>(smart_ptr::make_unique<FunctionWrapper<F>>(std::move(f)));
    }
    else {
        is_object_small = true;
        new (buffer) FunctionWrapper<F>(std::move(f));
    }
}
    
template <typename ReturnType, typename... Args>
Function<ReturnType (Args...)>::~Function(){
    clear();
}

// template <typename ReturnType, typename... Args>
// Function<ReturnType (Args...)>& Function<ReturnType (Args...)>::operator=(const Function& other_function) {
//     clear();

//     Function tmp(other_function);
//     swap(tmp);
//     return *this;
// }

template <typename ReturnType, typename... Args>
Function<ReturnType (Args...)>& Function<ReturnType (Args...)>::operator=(Function&& other_function) {
    clear();

    if (other_function.is_object_small) {
        reinterpret_cast<Wrapper*>(other_function.buffer)->move_by_ptr(buffer);
        new (other_function.buffer) smart_ptr::UniquePtr<Wrapper>(nullptr);
    }
    else {
        new (buffer) smart_ptr::UniquePtr(std::move(other_function.big_object_ptr));
    }
    is_object_small = other_function.is_object_small;
    other_function.is_object_small = false;

    return *this;
}

template <typename ReturnType, typename... Args>
void Function<ReturnType (Args...)>::swap(Function& other_function) noexcept {
    Function tmp(std::move(other_function));
    other_function = std::move(*this);
    *this = std::move(tmp);
}

template <typename ReturnType, typename... Args>
ReturnType Function<ReturnType (Args...)>::operator()(Args... args){
    if (!static_cast<bool>(*this)) {
        throw exception::LogicException("Invalid function call");
    }
    else{
        if (is_object_small) {
            return reinterpret_cast<Wrapper*>(buffer)->call(std::forward<Args>(args)...);
        } 
        else {
            return big_object_ptr->call(std::forward<Args>(args)...);
        }
    }
}

}