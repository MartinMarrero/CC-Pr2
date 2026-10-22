#pragma once

#include "TuringMachine.h"

#include <string>
#include <vector>

class InputReader {
public:
    static std::vector<std::string> readFromFile(const std::string& filename);
    static std::vector<std::string> readFromConsole();
    static void validateInput(const std::string& input, const TuringMachine& machine);
};
