#include "../../include/Tuple/Tuple.hpp"

#include <utility> // std::move

namespace tmn_tuple {

template <typename Type>
Tuple<Type>::Tuple(const Tuple<Type>& other) : head(other.head) { }

template <typename Type>
Tuple<Type>::Tuple(Tuple<Type>&& other) : head(std::move(other.head)) {}

template <typename Type>
Tuple<Type>::Tuple(const Type& value) : head(value) {}

template <typename Type>
Tuple<Type>::Tuple(Type&& value) : head(std::move(value)) {}

template <typename Type>
Tuple<Type>& Tuple<Type>::operator=(const Tuple<Type>& other) {
    head = other.head;
    return *this;
}

template <typename Type>
Tuple<Type>& Tuple<Type>::operator=(Tuple<Type>&& other) {
    head = std::move(other.head);
    return *this;
}

template <typename Type>
void Tuple<Type>::swap(Tuple<Type>& other) {
    std::swap(head, other.head);
}

template <typename Type>
std::size_t Tuple<Type>::size() {
    return 1;
}



template <typename HeadType, typename... TailTypes>
Tuple<HeadType, TailTypes...>::Tuple(const Tuple<HeadType, TailTypes...>& other) : head(other.head), others(other.others) {}

template <typename HeadType, typename... TailTypes>
Tuple<HeadType, TailTypes...>::Tuple(Tuple<HeadType, TailTypes...>&& other) : head(std::move(other.head)), others(std::move(other.others)) {}

template <typename HeadType, typename... TailTypes>
template <typename HType>
Tuple<HeadType, TailTypes...>::Tuple(HType&& head, TailTypes&& ... tail) : head(std::forward<HType>(head)), others(std::forward<TailTypes>(tail)...) {}

template <typename HeadType, typename... TailTypes>
Tuple<HeadType, TailTypes...>& Tuple<HeadType, TailTypes...>::operator=(const Tuple<HeadType, TailTypes...>& other) {
    head = other.head;
    others = other.others;
    return *this;
}

template <typename HeadType, typename... TailTypes>
Tuple<HeadType, TailTypes...>& Tuple<HeadType, TailTypes...>::operator=(Tuple<HeadType, TailTypes...>&& other) {
    head = std::move(other.head);
    others = std::move(other.others);
    return *this;
}

template <typename HeadType, typename... TailTypes>
void Tuple<HeadType, TailTypes...>::swap(Tuple<HeadType, TailTypes...>& other) {
    std::swap(head, other.head);
    std::swap(others, other.others);
}

template <typename HeadType, typename... TailTypes>
std::size_t Tuple<HeadType, TailTypes...>::size() {
    return 1 + others.size();
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Functors-helpers :
// get_by_num

template <size_t N, typename HeadT, typename... TailT>
decltype(auto) get_by_num<N, HeadT, TailT...>::discard(Tuple<HeadT, TailT...>& tuple) {
    return get_by_num<N-1, TailT...>::discard(tuple.others);
}

template <size_t N, typename HeadT, typename... TailT>
decltype(auto) get_by_num<N, HeadT, TailT...>::discard(const Tuple<HeadT, TailT...>& tuple) {
    return get_by_num<N-1, TailT...>::discard(tuple.others);
}

template <size_t N, typename HeadT, typename... TailT>
decltype(auto) get_by_num<N, HeadT, TailT...>::discard(Tuple<HeadT, TailT...>&& tuple) {
    return get_by_num<N-1, TailT...>::discard(std::move(tuple.others));
}

template <typename HeadT, typename... TailT>
HeadT& get_by_num<1, HeadT, TailT...>::discard(Tuple<HeadT, TailT...>& tuple) {
    return tuple.head;
}

template <typename HeadT, typename... TailT>
const HeadT& get_by_num<1, HeadT, TailT...>::discard(const Tuple<HeadT, TailT...>& tuple) {
    return tuple.head;
}

template <typename HeadT, typename... TailT>
HeadT&&  get_by_num<1, HeadT, TailT...>::discard(Tuple<HeadT, TailT...>&& tuple) {
    return std::move(tuple.head);
}

template <size_t N, typename HeadT, typename... TailT>
decltype(auto) get(Tuple<HeadT, TailT...>& tuple) {
    return get_by_num<N, HeadT, TailT...>::discard(tuple);
};

template <size_t N, typename HeadT, typename... TailT>
decltype(auto) get(const Tuple<HeadT, TailT...>& tuple) {
    return get_by_num<N, HeadT, TailT...>::discard(tuple);
};

template <size_t N, typename HeadT, typename... TailT>
decltype(auto) get(Tuple<HeadT, TailT...>&& tuple) {
    return get_by_num<N, HeadT, TailT...>::discard(std::move(tuple));
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Functors-helpers :
// get_by_type

template <typename T, typename HeadT, typename... TailT>
T& get_by_type<T, HeadT, TailT...>::discard(Tuple<HeadT, TailT...>& tuple) {
    return get_by_type<T, TailT...>::discard(tuple.others);
}

template <typename T, typename HeadT, typename... TailT>
const T& get_by_type<T, HeadT, TailT...>::discard(const Tuple<HeadT, TailT...>& tuple) {
    return get_by_type<T, TailT...>::discard(tuple.others);
}

template <typename T, typename HeadT, typename... TailT>
T&& get_by_type<T, HeadT, TailT...>::discard(Tuple<HeadT, TailT...>&& tuple) {
    return get_by_type<T, TailT...>::discard(std::move(tuple.others));
}

template <typename T, typename... TailT>
T& get_by_type<T, T, TailT...>::discard(Tuple<T, TailT...>& tuple) {
    return tuple.head;
}

template <typename T, typename... TailT>
const T& get_by_type<T, T, TailT...>::discard(const Tuple<T, TailT...>& tuple) {
    return tuple.head;
}

template <typename T, typename... TailT>
T&& get_by_type<T, T, TailT...>::discard(Tuple<T, TailT...>&& tuple) {
    return std::move(tuple.head);
}

template <typename T, typename HeadT, typename... TailT>
T& get(Tuple<HeadT, TailT...>& tuple) {
    return get_by_type<T, HeadT, TailT...>::discard(tuple);
}

template <typename T, typename HeadT, typename... TailT>
const T& get(const Tuple<HeadT, TailT...>& tuple) {
    return get_by_type<T, HeadT, TailT...>::discard(tuple);
}

template <typename T, typename HeadT, typename... TailT>
T&& get(Tuple<HeadT, TailT...>&& tuple) {
    return get_by_type<T, HeadT, TailT...>::discard(std::move(tuple));
}

}