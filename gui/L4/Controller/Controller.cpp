#include "Controller.hpp"

namespace tmn::visualize::graph {

template <bool is_graph_oriented, bool has_weight>
unchar ConsoleController<is_graph_oriented, has_weight>::main_menu() const noexcept {
    std::cout << "--- Main Menu ---" << std::endl;
    std::cout << "The current monitored graph: " get_graph_info() << std::endl;
    std::cout << "a: create a graph based on the given parameters" << std::endl;
    std::cout << "b: create a random graph" << std::endl;
    std::cout << "c: delete current graph" << std::endl;
    std::cout << "?: get help" << std::endl;
    std::cout << "!: ---> exit <---" << std::endl;
    std::cout << "Enter a character to continue" << std::endl;
    
    char choice = '0';
    int invalid_character_counter = 10;

    while (invalid_character_counter != 0){
        std::cout << "> ";
        std::cin >> choice;
        std::cout << std::endl;
        if (choice == 'a'){
            return 1;
        }
        else if (choice == 'b'){
            return 2;
        }
        else if (choice == 'c'){
            return 3;
        }
        else if (choice == '!'){
            return 0;
        }
        else{
            --invalid_character_counter;
            std::cout << "An incorrect character has been entered. The program will end if you enter 10 incorrect characters in a row" << std::endl;
        }
    }

    return 0;
}

template <bool is_graph_oriented, bool has_weight>
unchar ConsoleController<is_graph_oriented, has_weight>::creating_graph_by_parameters_menu() const noexcept {
    std::cout << "--- Creating graph by parameters menu ---" << std::endl;
    std::cout << "a: create a graph based on fixed parameters" << std::endl;
    std::cout << "b: create a graph by topology" << std::endl;
    std::cout << "!: ---> back <---" << std::endl;
    std::cout << "Enter a character to continue" << std::endl;
    
    char choice = '0';
    int invalid_character_counter = 10;

    while (invalid_character_counter != 0){
        std::cout << "> ";
        std::cin >> choice;
        std::cout << std::endl;
        if (choice == 'a'){
            return 1;
        }
        else if (choice == 'b'){
            return 2;
        }
        else if (choice == '!'){
            return 0;
        }
        else{
            --invalid_character_counter;
            std::cout << "An incorrect character has been entered. The program will return to the main menu if you enter 10 incorrect characters in a row" << std::endl;
        }
    }

    return 0;
}

// -----------------------------------------------------------------------------------------------------------------------------

bool good_number_of_v(const std::string& num){
    if (num.size() < 1 || num.size() > 3){
        return false;
    }
    for (char c : num) {
        if (!isdigit(c)){
            return false;
        }   
    }
    if (atoi(num.c_str())> 255){
        return false;
    }
    return true;
}

bool correct_pattern(const std::string& first, const std::string& second, int upb){
    
}

template <bool is_graph_oriented, bool has_weight>
void ConsoleController<is_graph_oriented, has_weight>::creating_graph_by_parameters() {
    std::cout << "--- Creating graph by parameters ---" << std::endl;
    std::cout << "Enter the number of vertices in the graph to be created (from 0 to 255): " << std::endl;
    
    
    int number_of_vertices = 0;
    std::string number_of_vertices_str = "";
    int invalid_input_counter = 10;

    std::cout << "> ";
    std::cin >> number_of_vertices_str;

    while (invalid_character_counter != 0 && !good_number_of_v(number_of_vertices_str)){
        --invalid_character_counter;
        
        if (invalid_input_counter % 5 == 0){
            std::cout << "An incorrect command has been entered. If you continue to enter incorrect commands, you will not be able to create a graph" << std::endl;
        }
        else{
            std::cout << std::endl << "Wrong command. Try again";}
        }

        std::cout << "> ";
        std::cin >> number_of_vertices_str;
        std::cout << std::endl;
    }

    if(invalid_character_counter == 0 || !good_number_of_v(number_of_vertices_str)){
        return;
    }

    std::cout << "Enter vertices and edges according to the following pattern:" << std::endl;
    std::cout << "[from] [to]" << std::endl;
    std::cout << "Where [from] and [to] are numbers with value 0 - <number of vertices, that you input recently> (not inclusive)" << std::endl;
    
    char choice = '0';
    int invalid_character_counter = 10;

    while (invalid_character_counter != 0){
        std::cout << "> ";
        std::cin >> choice;
        std::cout << std::endl;
        if (choice == 'a'){
            return 1;
        }
        else if (choice == 'b'){
            return 2;
        }
        else if (choice == '!'){
            return 0;
        }
        else{
            --invalid_character_counter;
            std::cout << "An incorrect character has been entered. The program will return to the main menu if you enter 10 incorrect characters in a row" << std::endl;
        }
    }
    return Optional<Graph<is_graph_oriented, unchar, VertexResource, void>>();
}


}