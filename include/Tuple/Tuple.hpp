#pragma once

namespace tmn_tuple {

template <typename Type>
class Tuple {
private:
// Fields :
    Type value;

public:
    Tuple();
    Tuple(const Tuple<T>&);
    Tuple(Tuple<T>&&);

    explicit Tuple(const T&...);
    explicit Tuple(T&&);

    Tuple& operator= (const Tuple&);
    Tuple& operator= (Tuple&&);
    
    ~Tuple();

    void swap(Tuple&);

private:
// Friends :
    template <size_t N, typename Head, typename... Others>
    friend class get_by_num;

    template <typename T, typename Head, typename... Others>
    friend class get_by_type;
};



template <typename HeadType, typename... TailTypes>
class Tuple {
private:
// Fields :
    HeadType head;
    Tuple<TailTypes...> others;

public:
// Constructors & assignment & conversion & destructor:
    Tuple();
    Tuple(const Tuple&);
    Tuple(Tuple&&);

    template <typename... Types>
    explicit Tuple(const HeadType&, Types&&...);
    template <typename... Types>
    explicit Tuple(HeadType&&, Types&&...);

    Tuple& operator= (const Tuple&);
    Tuple& operator= (Tuple&&);
    
    ~Tuple();

    void swap(Tuple&);

private:
// Friends :
    template <size_t N, typename Head, typename... Others>
    friend class get_by_num;

    template <typename T, typename Head, typename... Others>
    friend class get_by_type;
}




}