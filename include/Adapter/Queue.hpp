#pragma once

#include "../Sequence/ListSequence.hpp"

namespace tmn {
namespace adapter {

template< typename T, class Container = tmn::sequence::ListSequence<T>>
class Queue {
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
	Queue() : container() {}
	Queue(const Container& container) : container(container) {}
	Queue(const Queue<T, Container>& other) : container(other.container) {}
    Queue(std::initializer_list<T> lst) : container(lst) {}

    void swap(Queue<T, Container>& other);
    Queue<T, Container>& operator=(const Queue<T, Container>& other);

	~Queue() = default;

    // Capacity & size :
	std::size_t size() const noexcept;
	bool empty() const noexcept;

	Queue<T, Container>& push(const T& item);
	Queue<T, Container>& push(T&& item);
	T pop();
	Queue<T, Container>& clear();

    // Element access methods :
	const T& front() const;
	T& front();
};

}
}

#include "../../src/Adapter/Queue.tpp"