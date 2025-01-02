#include "include/Function/Function.hpp"

#include <iostream>

int square(int x){
    return x * x;
}

int main(){
    tmn::Function<int(int)> f = square;
    std::cout << f(12) << std::endl;
    return 0;
}