#include <SDL2/SDL.h>

#include "VisualSorters.hpp"

namespace tmn {
namespace sort {

void draw_state(tmn::sequence::Sequence<int>& seq, SDL_Renderer* renderer, unsigned int red, unsigned int blue){
    for (size_t i = 0; i < seq.size(); ++i){
        if (i == red){
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else if (i == blue){
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        }
        else{
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }
        SDL_RenderDrawLine(renderer, i, 99, i, seq[i] + 50);
    }
}

sequence::Sequence<int>& VisualHeapSort::operator()(sequence::Sequence<int>& sequence, CmpFunction cmp) const noexcept {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_CreateWindowAndRenderer(50*10, 100*10, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, 10, 10);

    int size = sequence.size();

    build_heap(sequence, cmp, renderer);

    for (int i = size - 1; i > 0; i--) {
        std::swap(sequence.get(0), sequence.get(i));
        heapify(sequence, i, 0, cmp, renderer);
    }

    if (renderer){
        SDL_DestroyRenderer(renderer);
    }
    if (window){
        SDL_DestroyWindow(window);
    }
    SDL_Quit();

    return sequence;
}

void VisualHeapSort::heapify(sequence::Sequence<int>& sequence, int n, int i, CmpFunction cmp, SDL_Renderer* renderer) const {
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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        draw_state(sequence, renderer, i, largest);
        SDL_RenderPresent(renderer);
        SDL_Delay(35);

        std::swap(sequence.get(i), sequence.get(largest));
        heapify(sequence, n, largest, cmp, renderer);
    }
}

void VisualHeapSort::build_heap(sequence::Sequence<int>& sequence, CmpFunction cmp, SDL_Renderer* renderer) const {
    int size = sequence.size();
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(sequence, size, i, cmp, renderer);
    }
}

sequence::Sequence<int>& VisualShellSort::operator()(sequence::Sequence<int>& sequence, CmpFunction cmp) const noexcept {
    size_t size = sequence.size();
    if (size <= 1) {
        return sequence;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_CreateWindowAndRenderer(50*10, 100*10, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, 10, 10);

    for (size_t gap = size / 2; gap > 0; gap /= 2) {
        for (size_t i = gap; i < size; i++) {
            int temp = sequence.get(i);
            size_t j;
            for (j = i; j >= gap && cmp(sequence.get(j - gap), temp) > 0; j -= gap) {
                sequence.get(j) = sequence.get(j - gap);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                draw_state(sequence, renderer, j, j - gap);
                SDL_RenderPresent(renderer);
                SDL_Delay(50);
            }
            sequence.get(j) = temp;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            draw_state(sequence, renderer, j, i);
            SDL_RenderPresent(renderer);
            SDL_Delay(35);
        }
    }

    if (renderer){
        SDL_DestroyRenderer(renderer);
    }
    if (window){
        SDL_DestroyWindow(window);
    }
    SDL_Quit();

    return sequence;
}

sequence::Sequence<int>& VisualQuickSort::operator()(sequence::Sequence<int>& sequence, CmpFunction cmp) const noexcept {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_CreateWindowAndRenderer(50*10, 100*10, 0, &window, &renderer);
    SDL_RenderSetScale(renderer, 10, 10);
    
    helper(sequence, 0, sequence.size() - 1, cmp, renderer);

    if (renderer){
        SDL_DestroyRenderer(renderer);
    }
    if (window){
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
    return sequence;
}

int VisualQuickSort::partition(sequence::Sequence<int> &sequence, int low, int high, CmpFunction cmp, SDL_Renderer* renderer) const {
    int pivot = sequence.get(high);
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++) {
        if (cmp(sequence.get(j), pivot) < 0) {
            i++;
            std::swap(sequence.get(i), sequence.get(j));
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            draw_state(sequence, renderer, i, j);
            SDL_RenderPresent(renderer);
            SDL_Delay(35);
        }
    }
    std::swap(sequence.get(i + 1), sequence.get(high));
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    draw_state(sequence, renderer, i + 1, high);
    SDL_RenderPresent(renderer);
    SDL_Delay(50);

    return (i + 1);
}

void VisualQuickSort::helper(sequence::Sequence<int>& sequence, int low, int high, CmpFunction cmp, SDL_Renderer* renderer) const {
    if (low < high) {
        int pi = partition(sequence, low, high, cmp, renderer);
        helper(sequence, low, pi - 1, cmp, renderer);
        helper(sequence, pi + 1, high, cmp, renderer);
    }
}

}
}