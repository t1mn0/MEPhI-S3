#pragma once

#include <SDL2/SDL.h>

#include "../../include/Sort/Sort.hpp"

namespace tmn {
namespace sort {

void draw_state(tmn::sequence::Sequence<int>& seq, SDL_Renderer* renderer, unsigned int red, unsigned int blue);

class VisualHeapSort : public HeapSort<int> {
public:
    using CmpFunction = Function<int(const int&, const int&)>;

    sequence::Sequence<int>& operator()(sequence::Sequence<int>& sequence, CmpFunction cmp = CmpFunction(default_comparator<int>)) const noexcept override;

private:
    void heapify(sequence::Sequence<int>& sequence, int n, int i, CmpFunction cmp, SDL_Renderer*) const;
    void build_heap(sequence::Sequence<int>& sequence, CmpFunction cmp, SDL_Renderer* renderer) const;
};

class VisualShellSort : public ShellSort<int> {
public:
    using CmpFunction = Function<int(const int&, const int&)>;

    sequence::Sequence<int>& operator()(sequence::Sequence<int>& sequence, CmpFunction = CmpFunction(default_comparator<int>)) const noexcept override;
};

class VisualQuickSort : public QuickSort<int> {
public:
    using CmpFunction = Function<int(const int&, const int&)>;
    sequence::Sequence<int>& operator()(sequence::Sequence<int>& sequence, CmpFunction = CmpFunction(default_comparator<int>)) const noexcept override;

private:
    void helper(sequence::Sequence<int>& sequence, int low, int high, CmpFunction, SDL_Renderer* renderer) const;
    int partition(sequence::Sequence<int>& sequence, int low, int high, CmpFunction, SDL_Renderer* renderer) const;
};

}
}