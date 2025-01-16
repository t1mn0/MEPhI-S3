#include <iostream>
#include <fstream>
#include <string>

#include "../../dependencies/gnuplot-iostream.h"

int main() {
    Gnuplot gp1;
    gp1 << "set title 'Сортировки последовательности типа SmartSequence<int>'" << std::endl;
    gp1 << "set xlabel 'Количество элементов (*10^4)'" << std::endl;
    gp1 << "set ylabel 'Время (микросекунды)'" << std::endl;
    gp1 << "plot '" << EXECUTABLE_PATH << "/TestResults/heapsort_smartseq.txt' with linespoints title 'HeapSort', \\\n"
        << "'" << EXECUTABLE_PATH << "/TestResults/shellsort_smartseq.txt' with linespoints title 'ShellSort', \\\n"
        << "'" << EXECUTABLE_PATH << "/TestResults/quicksort_smartseq.txt' with linespoints title 'QuickSort'" << std::endl;
    gp1 << "pause -1" << std::endl;

    Gnuplot gp2;
    gp2 << "set title 'Сортировки последовательности типа ArraySequence<int>'" << std::endl;
    gp2 << "set xlabel 'Количество вершин (*10^4)'" << std::endl;
    gp2 << "set ylabel 'Время (микросекунды)'" << std::endl;
    gp2 << "plot '" << EXECUTABLE_PATH << "/TestResults/heapsort_arrayseq.txt' with linespoints title 'HeapSort', \\\n"
        << "'" << EXECUTABLE_PATH << "/TestResults/shellsort_arrayseq.txt' with linespoints title 'ShellSort', \\\n"
        << "'" << EXECUTABLE_PATH << "/TestResults/quicksort_arrayseq.txt' with linespoints title 'QuickSort'" << std::endl;
    gp2 << "pause -1" << std::endl;

    return 0;
}
