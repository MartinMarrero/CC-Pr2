#include "OutputFormatter.h"

#include <iostream>
#include <sstream>

/** @brief Formats a simulation result with visible tape heads. */
std::string OutputFormatter::formatResult(const SimulationResult& result) {
    std::ostringstream output;
    output << (result.accepted ? "ACCEPTED" : "REJECTED")
           << " | state=" << result.finalState << " | steps=" << result.steps << '\n';
    for (std::size_t i = 0; i < result.tapeContents.size(); ++i) {
        const auto& tape = result.tapeContents[i];
        const auto head = result.headPositions[i];
        output << "tape " << (i + 1) << ": ";
        if (head >= 0 && static_cast<std::size_t>(head) < tape.size()) {
            output << tape.substr(0, static_cast<std::size_t>(head)) << '['
                   << tape[static_cast<std::size_t>(head)] << ']'
                   << tape.substr(static_cast<std::size_t>(head) + 1);
        } else {
            output << tape << " (head=" << head << ')';
        }
        output << '\n';
    }
    return output.str();
}

/** @brief Writes a formatted simulation result to standard output. */
void OutputFormatter::printResult(const SimulationResult& result) {
    std::cout << formatResult(result);
}
