#include "include/Associative/Map.hpp"
#include <iostream>

int main(){
    tmn_associative::Map<char, int> m1 = {{'a', 1}, {'b', 13}, {'c', 66}};
    std::cout << "SIZE: " << m1.size() << std::endl;
    std::cout << m1.insert({'a', 11}) << std::endl;
    std::cout << m1.insert({'d', 41}) << std::endl;
    std::cout << m1.insert({'D', 0}) << std::endl;
    std::cout << m1.insert({'d', 41}) << std::endl;
    std::cout << "SIZE: " << m1.size() << std::endl;
    m1.erase('d');
    std::cout << "SIZE: " << m1.size() << std::endl;
    m1.erase('c');
    std::cout << "SIZE: " << m1.size() << std::endl;
    m1.erase('c');
    std::cout << "SIZE: " << m1.size() << std::endl;

}