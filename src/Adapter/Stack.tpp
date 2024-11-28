#include "../../include/Adapter/Stack.hpp"



namespace tmn_adapter{

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion :

template <typename T, class Container>
void Stack<T, Container>::swap(Stack<T, Container>& other) {
    container.swap(other.container);
}

template <typename T, class Container>
Stack<T, Container>& Stack<T, Container>::operator=(const Stack<T, Container>& other) {
    Stack<T, Container> new_stack(other);
    swap(new_stack);
    return *this;
}

template <typename T, class Container>
std::size_t Stack<T, Container>::size() const noexcept {
    return container.size();
}

template <typename T, class Container>
bool Stack<T, Container>::empty() const noexcept {
    return container.empty();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Capacity & size :

template <typename T, class Container>
Stack<T, Container>& Stack<T, Container>::push(const T& item) {
    container.push_back(item);
    return *this;
}

template <typename T, class Container>
Stack<T, Container>& Stack<T, Container>::push(T&& item){
    container.push_back(item);
    return *this;
}

template <typename T, class Container>
T Stack<T, Container>::pop() {
    T res = container.back();
    container.pop_back();
    return res;
}

template <typename T, class Container>
Stack<T, Container>& Stack<T, Container>::clear() {
    container.clear();
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Element access methods :

template <typename T, class Container>
const T Stack<T, Container>::top() const {
    return container.back();
}

template <typename T, class Container>
T Stack<T, Container>::top() {
    return container.back();
}

}