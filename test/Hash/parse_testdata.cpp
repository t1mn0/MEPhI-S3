#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

using namespace std;

int main() {
    std::string input_file_ht_str = std::string(EXECUTABLE_PATH) + "/load_test_hashtable.txt";
    std::string output_file_ht_str = std::string(EXECUTABLE_PATH) + "/output_load_test_hashtable.txt";
    std::string input_file_um_str = std::string(EXECUTABLE_PATH) + "/load_test_unorderedmap.txt";
    std::string output_file_um_str = std::string(EXECUTABLE_PATH) + "/output_load_test_unorderedmap.txt";

    ifstream input_file_ht(input_file_ht_str.c_str());
    ofstream output_file_ht(output_file_ht_str.c_str());

    ifstream input_file_um(input_file_um_str.c_str());
    ofstream output_file_um(output_file_um_str.c_str());

    if (!input_file_ht.is_open()) {
        cerr << "Failed to open input file: " << input_file_ht_str << endl;
        return 1;
    }
    if (!output_file_ht.is_open()) {
        cerr << "Failed to open the output file: " << output_file_ht_str << endl;
        return 1;
    }
    if (!input_file_um.is_open()) {
        cerr << "Failed to open input file: " << input_file_um_str << endl;
        return 1;
    }
    if (!output_file_um.is_open()) {
        cerr << "Failed to open the output file: " << output_file_um_str << endl;
        return 1;
    }

    string line;
    regex nanoseconds_regex("(\\d+) nanoseconds");
    smatch match;

    while (getline(input_file_ht, line)) {
        if (regex_search(line, match, nanoseconds_regex)) {
            output_file_ht << match[1] << endl;
        }
    }

    while (getline(input_file_um, line)) {
        if (regex_search(line, match, nanoseconds_regex)) {
            output_file_um << match[1] << endl;
        }
    }

    input_file_ht.close();
    output_file_ht.close();
    input_file_um.close();
    output_file_um.close();

    return 0;
}
