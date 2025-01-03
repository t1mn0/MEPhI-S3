#include <iostream>
#include <fstream>
#include <string>

#include "../../dependencies/gnuplot-iostream.h"

int main() {
    Gnuplot gp1;
    gp1 << "set title 'Алгоритмы для графа без весов'" << std::endl;
    gp1 << "set xlabel 'Количество вершин (*10^2)'" << std::endl;
    gp1 << "set ylabel 'Время (миллисекунды)'" << std::endl;
    gp1 << "plot '" << EXECUTABLE_PATH << "/load_test_bfs_unweighted_graph.txt' with linespoints title 'BFS', \\\n"
        << "'" << EXECUTABLE_PATH << "/load_test_dfs_unweighted_graph.txt' with linespoints title 'DFS', \\\n"
        << "'" << EXECUTABLE_PATH << "/load_test_coloring_unweighted_graph.txt' with linespoints title 'COLORING'" << std::endl;
    gp1 << "pause -1" << std::endl;

    Gnuplot gp2;
    gp2 << "set title 'Алгоритмы для графа c весом'" << std::endl;
    gp2 << "set xlabel 'Количество вершин (*10^2)'" << std::endl;
    gp2 << "set ylabel 'Время (миллисекунды)'" << std::endl;
    gp2 << "plot '" << EXECUTABLE_PATH << "/load_test_bfs_singleweighted_graph.txt' with linespoints title 'BFS', \\\n"
        << "'" << EXECUTABLE_PATH << "/load_test_dfs_singleweighted_graph.txt' with linespoints title 'DFS', \\\n"
        << "'" << EXECUTABLE_PATH << "/load_test_coloring_singleweighted_graph.txt' with linespoints title 'COLORING'" << std::endl;
    gp2 << "pause -1" << std::endl;

    return 0;
}
