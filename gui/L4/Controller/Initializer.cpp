#include <iostream>

#include "Controller.hpp" 

namespace tmn::visualize::graph {

// -----------------------------------------------------------------------------------------------------------------------------

Pair<bool, bool> init() {
    char choice = '@';

    std::cout << "Initialization process..." << std::endl;
    std::cout << "If you would like help on how to use this program, enter 'h' (help)" << std::endl;
    std::cout << "To continue without getting help, enter 'n' (next)" << std::endl;
    std::cout << "> ";
    std::cin >> choice;
    std::cout << std::endl;

    if (choice == 'h'){
        global_help();
        std::cout << "You can always get help by calling this command" << std::endl;
        std::cout << "Continue? (y)" << std::endl;
        std::cout << "> ";
        std::cin >> choice;
    }

    char choice_direct_type = '@';
    bool oriented_graph = false;

    std::cout << "Select graph type [you will only be able to work with the selected graph type] :" << std::endl;
    std::cout << "1) oriented graph" << std::endl;
    std::cout << "2) no-oriented graph" << std::endl;
    std::cout << "> ";
    std::cin >> choice_direct_type;

    while (choice_direct_type != '1' && choice_direct_type != '2'){
        std::cout << std::endl << "Wrong command. Try again";
        std::cout << "> ";
        std::cin >> choice_direct_type;
    }

    oriented_graph = choice_direct_type == '1';
    
    char choice_weight_type = '@';
    bool has_weight = false;

    std::cout << "Select graph type [you will only be able to work with the selected graph type] :" << std::endl;
    std::cout << "1) weighted graph" << std::endl;
    std::cout << "2) non-weighted graph" << std::endl;
    std::cout << "> ";
    std::cin >> choice_weight_type;

    while (choice_weight_type != '1' && choice_weight_type != '2'){
        std::cout << std::endl << "Wrong command. Try again";
        std::cout << "> ";
        std::cin >> choice_weight_type;
    }

    has_weight = choice_weight_type == '1';

    return Pair<bool, bool>(oriented_graph, choice_weight_type);
}

// -----------------------------------------------------------------------------------------------------------------------------

void run() {
    Pair<bool, bool> init_data = init();

}

// -----------------------------------------------------------------------------------------------------------------------------

void global_help() {
    std::cout << "bla bla bla" << std::endl;
}

// -----------------------------------------------------------------------------------------------------------------------------

}