#include "statistics.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: signal_analyzer <measurements.txt>\n";
        return 2;
    }

    std::ifstream input(argv[1]);
    if (!input) {
        std::cerr << "Cannot open input file: " << argv[1] << '\n';
        return 2;
    }

    std::vector<double> values;
    std::string line;
    std::size_t line_number = 0;

    while (std::getline(input, line)) {
        ++line_number;
        if (line.find_first_not_of(" \t\r") == std::string::npos) {
            std::cerr << "Empty line at " << line_number << '\n';
            return 2;
        }

        std::istringstream parser(line);
        double value{};
        char extra{};
        if (!(parser >> value) || (parser >> extra)) {
            std::cerr << "Invalid measurement at line " << line_number << '\n';
            return 2;
        }
        values.push_back(value);
    }

    if (input.bad()) {
        std::cerr << "I/O error while reading input file\n";
        return 2;
    }

    try {
        const auto stats = calculate_statistics(values);
        std::cout << std::fixed << std::setprecision(6)
                  << "count: " << values.size() << '\n'
                  << "min: " << stats.minimum << '\n'
                  << "max: " << stats.maximum << '\n'
                  << "range: " << stats.range << '\n'
                  << "mean: " << stats.mean << '\n';
    } catch (const std::invalid_argument& error) {
        std::cerr << "Invalid measurements: " << error.what() << '\n';
        return 2;
    }

    return 0;
}
