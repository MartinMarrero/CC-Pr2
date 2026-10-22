#pragma once

#include <cstddef>
#include <string>
#include <vector>

struct SimulationResult {
    bool accepted = false;
    std::string finalState;
    std::vector<std::string> tapeContents;
    std::vector<long long> headPositions;
    std::size_t steps = 0;
};
