#pragma once

#include "SimulationResult.h"

#include <string>

class OutputFormatter {
public:
    static std::string formatResult(const SimulationResult& result);
    static void printResult(const SimulationResult& result);
};
