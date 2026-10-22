#include "Transition.h"

#include <utility>

/** @brief Creates a transition of the transition function. */
Transition::Transition(std::string currentState, std::vector<char> readSymbols,
                       std::string nextState, std::vector<char> writeSymbols,
                       std::vector<char> movements)
    : currentState_(std::move(currentState)),
      readSymbols_(std::move(readSymbols)),
      nextState_(std::move(nextState)),
      writeSymbols_(std::move(writeSymbols)),
      movements_(std::move(movements)) {}

/** @brief Returns the source state. */
const std::string& Transition::getCurrentState() const { return currentState_; }
/** @brief Returns the symbols required on each tape. */
const std::vector<char>& Transition::getReadSymbols() const { return readSymbols_; }
/** @brief Returns the destination state. */
const std::string& Transition::getNextState() const { return nextState_; }
/** @brief Returns the symbols written on each tape. */
const std::vector<char>& Transition::getWriteSymbols() const { return writeSymbols_; }
/** @brief Returns the head movements for each tape. */
const std::vector<char>& Transition::getMovements() const { return movements_; }
