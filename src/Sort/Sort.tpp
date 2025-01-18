#include <cmath>

#include "../../include/Sort/Sort.hpp"

namespace tmn {
namespace sort {

template <typename T>
int default_comparator(const T& a, const T& b) {
    if (a < b) return -1; 
    if (a > b) return 1;
    return 0;
}

template <typename T>
auto InsertionSort<T>::operator()(sequence::Sequence<T>& sequence, CmpFunction& cmp) const noexcept
    -> sequence::Sequence<T>& {

    size_t size = sequence.size();

    if (size <= 1) {
        return sequence;
    }

    for (size_t i = 1; i < size; ++i) {
        T key = sequence.get(i);

        int j = i - 1;
        while (j >= 0 && cmp(sequence.get(j), key) >  0) {
            sequence.get(j + 1) = sequence.get(j);
            --j;
        }
        sequence.get(j + 1) = key;
    }

    return sequence;
}

template <typename T>
auto InsertionSort<T>::operator()(sequence::Sequence<T>& sequence) const noexcept
    -> sequence::Sequence<T>& {

    CmpFunction def_cmp(default_comparator<T>);
    return operator()(sequence, def_cmp);
}

template <typename T>
auto ShellSort<T>::operator()(sequence::Sequence<T>& sequence, CmpFunction& cmp) const noexcept
    -> sequence::Sequence<T>& {

    return operator()(sequence, 2, cmp);
}

template <typename T>
auto ShellSort<T>::operator()(sequence::Sequence<T>& sequence) const noexcept
    -> sequence::Sequence<T>& {

    CmpFunction def_cmp(default_comparator<T>);
    return operator()(sequence, 2, def_cmp);
}

template <typename T>
auto ShellSort<T>::operator()(sequence::Sequence<T>& sequence, size_t gap_coef, CmpFunction& cmp) const noexcept
    -> sequence::Sequence<T>& {

    if (gap_coef <= 1){
        throw exception::LogicException("Gap coefficient should be greater than 1");
    }

    size_t size = sequence.size();
    if (size <= 1) {
        return sequence;
    }

    for (size_t gap = size / gap_coef; gap > 0; gap /= gap_coef) {
        for (size_t i = gap; i < size; i++) {
            T temp = sequence.get(i);
            size_t j;
            for (j = i; j >= gap && cmp(sequence.get(j - gap), temp) > 0; j -= gap) {
                sequence.get(j) = sequence.get(j - gap) ;
            }
            sequence.get(j) = temp;
        }
    }

    return sequence;
}

template <typename T>
auto ShellSort<T>::operator()(sequence::Sequence<T>& sequence, size_t gap_coef) const noexcept
    -> sequence::Sequence<T>& {

    CmpFunction def_cmp(default_comparator<T>);
    return operator()(sequence, gap_coef, def_cmp);
}

template <typename T>
void QuickSort<T>::helper(sequence::Sequence<T>& sequence, int low, int high, CmpFunction& cmp) const {
    if (low < high) {
        int pi = partition(sequence, low, high, cmp);
        helper(sequence, low, pi - 1, cmp);
        helper(sequence, pi + 1, high, cmp);
    }
}

template <typename T>
int QuickSort<T>::partition(sequence::Sequence<T>& sequence, int low, int high, CmpFunction& cmp) const {
    T pivot = sequence.get(high);
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (cmp(sequence.get(j), pivot) < 0) {
            i++;
            std::swap(sequence.get(i), sequence.get(j));
        }
    }
    std::swap(sequence.get(i + 1), sequence.get(high));

    return (i + 1);
}

template <typename T>
auto QuickSort<T>::operator()(sequence::Sequence<T>& sequence, CmpFunction& cmp) const noexcept 
    -> sequence::Sequence<T>& {

    helper(sequence, 0, sequence.size() - 1, cmp);
    return sequence;
}

template <typename T>
auto QuickSort<T>::operator()(sequence::Sequence<T>& sequence) const noexcept 
    -> sequence::Sequence<T>& {

    CmpFunction def_cmp(default_comparator<T>);
    return operator()(sequence, def_cmp);
}

template <typename T>
void HeapSort<T>::heapify(sequence::Sequence<T>& sequence, int n, int i, CmpFunction& cmp) const {
    int largest = i; 
    int l = 2 * i + 1; 
    int r = 2 * i + 2;
    
    if (l < n && cmp(sequence.get(l), sequence.get(largest)) > 0) {
        largest = l;
    }
    
    if (r < n && cmp(sequence.get(r), sequence.get(largest)) > 0) {
        largest = r;
    }

    if (largest != i) {
        std::swap(sequence.get(i), sequence.get(largest));
        heapify(sequence, n, largest, cmp);
    }
}

template <typename T>
void  HeapSort<T>::build_heap(sequence::Sequence<T>& sequence, CmpFunction& cmp) const{
    int size = sequence.size();
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(sequence, size, i, cmp);
    }
}

template <typename T>
auto HeapSort<T>::operator()(sequence::Sequence<T>& sequence, CmpFunction& cmp) const noexcept
    -> sequence::Sequence<T>& {

    int size = sequence.size();
    build_heap(sequence, cmp);
    for (int i = size - 1; i > 0; i--) {
    std::swap(sequence.get(0), sequence.get(i));
        heapify(sequence, i, 0, cmp);
    }
    return sequence;
}

template <typename T>
auto HeapSort<T>::operator()(sequence::Sequence<T>& sequence) const noexcept
    -> sequence::Sequence<T>& {

    CmpFunction def_cmp(default_comparator<T>);
    return operator()(sequence, def_cmp);
}

}
}