#pragma once

#include <string>
#include <vector>

class Transition {
public:
    Transition(std::string currentState, std::vector<char> readSymbols,
               std::string nextState, std::vector<char> writeSymbols,
               std::vector<char> movements);

    const std::string& getCurrentState() const;
    const std::vector<char>& getReadSymbols() const;
    const std::string& getNextState() const;
    const std::vector<char>& getWriteSymbols() const;
    const std::vector<char>& getMovements() const;

private:
    std::string currentState_;
    std::vector<char> readSymbols_;
    std::string nextState_;
    std::vector<char> writeSymbols_;
    std::vector<char> movements_;
};
