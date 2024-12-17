#include <string>
#include <iostream>

#include "../../include/Sequence/ArraySequence.hpp"

using Matrix = tmn_sequence::ArraySequence<tmn_sequence::ArraySequence<int>>;

template <typename VertexId>
void pretty_print_matrix(int num, const std::string& test_name, const Matrix& matrix, const tmn_sequence::ArraySequence<VertexId>& seq){
    std::cout << "["+ test_name + "] --- Graph" + std::to_string(num) + " ---" << std::endl << "   ";
    
    for (auto& id : seq) {
        std::cout << id << "  ";
    }
    std::cout << std::endl;

    std::size_t k = 0;
    for (auto& i : matrix){
        std::cout << seq[k] << " ";
        for (auto& j : i){
            if (j >= 0) {
                std::cout << " " << j << " ";
            }
            else{
                std::cout << j << " ";
            }
        }
        ++k;
        std::cout << std::endl;
    }
}

