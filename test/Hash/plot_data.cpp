#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#include "../../dependencies/gnuplot-iostream.h"

int main() {
    Gnuplot gp;
    gp << "set title 'Время вставки в хэш-таблицу'" << std::endl;
    gp << "set xlabel 'Номер теста (1 тест показывает время выполнения 1000 вставок)'" << std::endl;
    gp << "set ylabel 'Время (нс)'" << std::endl;
    gp << "plot '" << EXECUTABLE_PATH << "/output_load_test_hashtable.txt' with linespoints title 'HashTable (моя реализация)', \\\n"
       << "'" << EXECUTABLE_PATH "/output_load_test_unorderedmap.txt' with linespoints title 'std::unorderedmap'" << std::endl;
    gp << "pause -1" << std::endl;

    return 0;
}

