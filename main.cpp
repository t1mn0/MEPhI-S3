#include "include/Sort/Sort.hpp"
#include <iostream>

int main(){
    using namespace tmn;
    sequence::SmartSequence<int> sequence = {544, 45, -12, 432, 423, 456423, 0, 0, 1234};
    sort::QuickSort<int> sort;
    sort(sequence);
    for (size_t i = 0; i < sequence.size(); ++i){
        std::cout << sequence[i] << " ";
    }
    std::cout << '\n';
    return 0;
}