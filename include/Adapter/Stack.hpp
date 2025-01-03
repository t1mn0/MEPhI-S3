#pragma once

#include "../Sequence/ArraySequence.hpp"

namespace tmn {
namespace adapter {

template< typename T, class Container = tmn::sequence::ArraySequence<T>>
class Stack {
private:
	Container container;

public:
    // Using's :
    using container_type = Container;
    using value_type = T;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;

    // Constructors & assignment & conversion :
	Stack() : container() {}
	Stack(const Container& container) : container(container) {}
	Stack(const Stack<T, Container>& other) : container(other.container) {}
    Stack(std::initializer_list<T> lst) : container(lst) {}

    void swap(Stack<T, Container>& other);
    Stack<T, Container>& operator=(const Stack<T, Container>& other);

	~Stack() = default;

    // Capacity & size :
	std::size_t size() const noexcept;
	bool empty() const noexcept;

	Stack<T, Container>& push(const T& item);
    Stack<T, Container>& push(T&& item);
	T pop();
	Stack<T, Container>& clear();

    // Element access methods :
	const T& top() const;
	T& top();
};

}
}


#include "../../src/Adapter/Stack.tpp"