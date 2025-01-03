#include "../../include/Adapter/Queue.hpp"

namespace tmn {
namespace adapter {

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Constructors & assignment & conversion :

template <typename T, class Container>
void Queue<T, Container>::swap(Queue<T, Container>& other) {
    container.swap(other.container);
}

template <typename T, class Container>
Queue<T, Container>& Queue<T, Container>::operator=(const Queue<T, Container>& other) {
    Queue<T, Container> new_queue(other);
    swap(new_queue);
    return *this;
}

template <typename T, class Container>
std::size_t Queue<T, Container>::size() const noexcept {
    return container.size();
}

template <typename T, class Container>
bool Queue<T, Container>::empty() const noexcept {
    return container.empty();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Capacity & size :

template <typename T, class Container>
Queue<T, Container>& Queue<T, Container>::push(const T& item) {
    container.push_back(item);
    return *this;
}

template <typename T, class Container>
Queue<T, Container> &tmn::adapter::Queue<T, Container>::push(T&& item) {
    container.push_back(item);
    return *this;
}

template <typename T, class Container>
T Queue<T, Container>::pop() {
    T res = container.front();
    container.pop_front();
    return res;
}

template <typename T, class Container>
Queue<T, Container>& Queue<T, Container>::clear() {
    container.clear();
    return *this;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Element access methods :

template <typename T, class Container>
const T& Queue<T, Container>::front() const {
    return container.back();
}

template <typename T, class Container>
T& Queue<T, Container>::front() {
    return container.front();
}

}
}