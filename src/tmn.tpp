#include "../include/Exceptions/Exception.hpp"

#include <utility>

namespace tmn {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Conditional

template <bool B, typename U, typename V>
struct conditional{
    using type = V;
};

template <typename U, typename V>
struct conditional<true, U, V>{
    using type = U;
};

template <bool B, typename U, typename V>
using conditional_t = typename conditional<B, U, V>::type;

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Swap

template <typename T>
void swap(T& x, T& y){
    T tmp = std::move(x);
    x = std::move(y);
    y = std::move(tmp);
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// Move & forward

template <typename T>
std::remove_reference_t<T>&& move(T&& x) noexcept {
    return static_cast<std::remove_reference_t<T>&&>(x);
}

// rvalue - - - |
//              | - - - (move) - - - > rvalue
// lvalue - - - |

// A universal reference is such if it is an accepted argument to a function. 
// The output type T&& is not a universal reference (!)
// Universal Link Terms:
// 1) A template parameter of a function (not a class (!)), i.e. template <typename T> func (T&& . . . .) { . . . . . }
// 2) T&& type
// 3) Arguments of the function

// rvalue - - - |                       | - - - > rvalue
//              | - - - (forward) - - - | 
// lvalue - - - |                       | - - - > lvalue

template <typename T>
T&& forward(std::remove_reference_t<T>& x) noexcept {    
	return static_cast<T&&>(x);
}

// Function overload when forward accepts the rvalue-result of the function
template <typename T>
T&& forward(std::remove_reference_t<T>&& x) noexcept {
    static_assert(!std::is_reference_v<T>);
	return static_cast<T&&>(x);
}


// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// default_delete

template <typename T>
struct default_delete {
    default_delete() noexcept = default;
    void operator()(T* ptr) const {
        delete ptr;
    }
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
// any and idiom "type erasure"

class any{
private:
    struct Base {
        virtual Base* get_copy() const = 0;
        virtual ~Base() = 0;
    };

    template <typename T>
    struct Derived : public Base {
        T value;
        Derived(const T& value) : value(value) {}
        Derived(T&& value) : value(std::move(value)) {}
        ~Derived() = default;

        Base* get_copy() const override {
            return new Derived(value);
        }
    };

private:
    Base* ptr;

public:
    template <typename T>  
    any(const T& value) : ptr(new Derived<T>(value)) {}

    any(const any& other) : ptr(other.ptr->get_copy()) {}
    
    ~any(){
        delete ptr;
    }
};

template <typename T>
T& any_cast(any& a){
    auto* p = dynamic_cast<any::Derived<std::remove_reference_t<T>>>(a.ptr);
    if (!p) throw tmn_exception::Exception("Bad any cast");
    return p->value;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

}