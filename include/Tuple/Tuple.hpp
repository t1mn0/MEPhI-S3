#pragma once

#include <cstddef> // std::size_t

#include "../Optional/Optional.hpp"

namespace tmn_tuple {

template <typename HeadType, typename... TailTypes>
class Tuple {
private:
// Fields :
    HeadType head;
    Tuple<TailTypes...> others;

public:
// Constructors & assignment & conversion & destructor:
    Tuple() = default;
    Tuple(const Tuple<HeadType, TailTypes...>&);
    Tuple(Tuple<HeadType, TailTypes...>&&);
    
    template <typename HType = HeadType>
    explicit Tuple(HType&&, TailTypes&&...);

    Tuple& operator= (const Tuple&);
    Tuple& operator= (Tuple&&);
    
    ~Tuple() = default;

    void swap(Tuple&);

    std::size_t size();

private:
// Friends :
    template <std::size_t N, typename HeadT, typename... TailT>
    friend class get_by_num;

    template <typename T, typename HeadT, typename... TailT>
    friend class get_by_type;
};

template <typename Type>
class Tuple<Type> {
private:
// Fields :
    Type head;

public:
    Tuple();
    Tuple(const Tuple<Type>&);
    Tuple(Tuple<Type>&&);

    explicit Tuple(const Type&);
    explicit Tuple(Type&&);

    Tuple& operator= (const Tuple<Type>&);
    Tuple& operator= (Tuple<Type>&&);
    
    ~Tuple() = default;

    void swap(Tuple&);

    std::size_t size();

private:
// Friends :
    template <std::size_t N, typename HeadType, typename... TailTypes>
    friend class get_by_num;

    template <typename T, typename HeadType, typename... TailTypes>
    friend class get_by_type;
};

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Functors-helpers :
// get_by_num

template <size_t N, typename HeadT, typename... TailT>
struct get_by_num {
    static decltype(auto) discard(Tuple<HeadT, TailT...>& tuple);
   
    static decltype(auto) discard(const Tuple<HeadT, TailT...>& tuple);

    static decltype(auto) discard(Tuple<HeadT, TailT...>&& tuple) ;
};

template <typename HeadT, typename... TailT>
struct get_by_num<1, HeadT, TailT...> {
    static HeadT& discard(Tuple<HeadT, TailT...>& tuple);

    static const HeadT& discard(const Tuple<HeadT, TailT...>& tuple);

    static HeadT&& discard(Tuple<HeadT, TailT...>&& tuple);
};

template <size_t N, typename HeadT, typename... TailT>
decltype(auto) get(Tuple<HeadT, TailT...>& tuple);

template <size_t N, typename HeadT, typename... TailT>
decltype(auto) get(const Tuple<HeadT, TailT...>& tuple);

template <size_t N, typename HeadT, typename... TailT>
decltype(auto) get(Tuple<HeadT, TailT...>&& tuple);

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// Functors-helpers :
// get_by_type

template <typename T, typename HeadT, typename... TailT>
struct get_by_type {
    static T& discard(Tuple<HeadT, TailT...>& tuple);

    static const T& discard(const Tuple<HeadT, TailT...>& tuple);

    static T&& discard(Tuple<HeadT, TailT...>&& tuple);
};

template <typename T, typename... TailT>
struct get_by_type<T, T, TailT...> {
    static T& discard(Tuple<T, TailT...>& tuple);

    static const T& discard(const Tuple<T, TailT...>& tuple);

    static T&& discard(Tuple<T, TailT...>&& tuple);
};

template <typename T, typename HeadT, typename... TailT>
T& get(Tuple<HeadT, TailT...>& tuple);

template <typename T, typename HeadT, typename... TailT>
const T& get(const Tuple<HeadT, TailT...>& tuple);

template <typename T, typename HeadT, typename... TailT>
T&& get(Tuple<HeadT, TailT...>&& tuple);

}

#include "../../src/Tuple/Tuple.tpp"