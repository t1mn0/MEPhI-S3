#pragma once

#include "SmartSequence.hpp"
#include "../Function/Function.hpp"

namespace tmn {
namespace sort {

template <typename T>
int default_comparator(const T& a, const T& b);

template <typename T>
class ISorter {
public:
    using CmpFunction = Function<int(const T&, const T&)>;

    virtual sequence::Sequence<T>& operator()(sequence::Sequence<T>&, CmpFunction = CmpFunction(default_comparator<T>)) const noexcept = 0;
    virtual ~ISorter() = default;
};

template <typename T>
class InsertionSort : public ISorter<T> {
public:
    using CmpFunction = Function<int(const T&, const T&)>;

    sequence::Sequence<T>& operator()(sequence::Sequence<T>& sequence, CmpFunction = CmpFunction(default_comparator<T>)) const noexcept override;
};

template <typename T>
class ShellSort : public ISorter<T> {
public:
    using CmpFunction = Function<int(const T&, const T&)>;

    sequence::Sequence<T>& operator()(sequence::Sequence<T>& sequence, size_t gap_coef, CmpFunction = CmpFunction(default_comparator<T>)) const;
    sequence::Sequence<T>& operator()(sequence::Sequence<T>& sequence, CmpFunction = CmpFunction(default_comparator<T>)) const noexcept;
};

template <typename T>
class QuickSort : public ISorter<T> {
public:
    using CmpFunction = Function<int(const T&, const T&)>;
    sequence::Sequence<T>& operator()(sequence::Sequence<T>& sequence, CmpFunction = CmpFunction(default_comparator<T>)) const noexcept override;

private:
    void helper(sequence::Sequence<T>& sequence, int low, int high, CmpFunction) const;
    int partition(sequence::Sequence<T>& sequence, int low, int high, CmpFunction) const;
};

template <typename T>
class HeapSort : public ISorter<T> {
public:
    using CmpFunction = Function<int(const T&, const T&)>;
    sequence::Sequence<T>& operator()(sequence::Sequence<T>& sequence, CmpFunction cmp = CmpFunction(default_comparator<T>)) const noexcept override;

private:
    void heapify(sequence::Sequence<T>& sequence, int n, int i, CmpFunction cmp) const;
    void build_heap(sequence::Sequence<T>& sequence, CmpFunction cmp) const;
};

}
}

#include "../../src/Sort/Sort.tpp"