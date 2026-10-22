#include "InputReader.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

/** @brief Reads one input string per line from a file. */
std::vector<std::string> InputReader::readFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Cannot open input file: " + filename);
    std::vector<std::string> inputs;
    std::string input;
    while (std::getline(file, input)) {
        if (!input.empty() && input.back() == '\r') input.pop_back();
        inputs.push_back(input);
    }
    return inputs;
}

/** @brief Reads non-empty input lines from standard input. */
std::vector<std::string> InputReader::readFromConsole() {
    std::vector<std::string> inputs;
    std::string input;
    while (std::getline(std::cin, input) && !input.empty()) inputs.push_back(input);
    return inputs;
}

/** @brief Verifies that every input symbol belongs to Sigma. */
void InputReader::validateInput(const std::string& input, const TuringMachine& machine) {
    for (char symbol : input) {
        if (!machine.getInputAlphabet().count(symbol)) {
            throw std::invalid_argument("Input contains symbol outside the input alphabet: " +
                                        std::string(1, symbol));
        }
    }
}
