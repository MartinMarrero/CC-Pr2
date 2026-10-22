#include "TuringMachine.h"

#include <stdexcept>

/** @brief Adds a state to the machine definition. */
void TuringMachine::addState(const std::string& state) { states_.insert(state); }
/** @brief Replaces the set of declared states. */
void TuringMachine::setStates(const std::set<std::string>& states) { states_ = states; }
/** @brief Sets the input alphabet. */
void TuringMachine::setInputAlphabet(const std::set<char>& alphabet) { inputAlphabet_ = alphabet; }
/** @brief Sets the tape alphabet. */
void TuringMachine::setTapeAlphabet(const std::set<char>& alphabet) { tapeAlphabet_ = alphabet; }
/** @brief Sets the initial state. */
void TuringMachine::setInitialState(const std::string& state) { initialState_ = state; }
/** @brief Sets the blank symbol. */
void TuringMachine::setBlankSymbol(char symbol) { blankSymbol_ = symbol; }
/** @brief Adds an accepting state. */
void TuringMachine::addAcceptingState(const std::string& state) { acceptingStates_.insert(state); }
/** @brief Sets the number of tapes and rejects zero. */
void TuringMachine::setNumberOfTapes(std::size_t numberOfTapes) {
    if (numberOfTapes == 0) throw std::invalid_argument("The number of tapes must be positive");
    numberOfTapes_ = numberOfTapes;
}
/** @brief Adds a transition to the machine definition. */
void TuringMachine::addTransition(const Transition& transition) { transitions_.push_back(transition); }

/** @brief Validates alphabets, states, transitions, arity, and determinism. */
void TuringMachine::validate() const {
    if (states_.empty() || initialState_.empty() || !states_.count(initialState_)) {
        throw std::invalid_argument("Initial state must belong to a non-empty state set");
    }
    if (tapeAlphabet_.empty() || !tapeAlphabet_.count(blankSymbol_)) {
        throw std::invalid_argument("Blank symbol must belong to the tape alphabet");
    }
    for (char symbol : inputAlphabet_) {
        if (!tapeAlphabet_.count(symbol)) throw std::invalid_argument("Input symbol is not on tape alphabet");
    }
    for (const auto& state : acceptingStates_) {
        if (!states_.count(state)) throw std::invalid_argument("Accepting state is not declared");
    }
    for (std::size_t i = 0; i < transitions_.size(); ++i) {
        const auto& transition = transitions_[i];
        if (!states_.count(transition.getCurrentState()) || !states_.count(transition.getNextState())) {
            throw std::invalid_argument("Transition references an undeclared state");
        }
        if (transition.getReadSymbols().size() != numberOfTapes_ ||
            transition.getWriteSymbols().size() != numberOfTapes_ ||
            transition.getMovements().size() != numberOfTapes_) {
            throw std::invalid_argument("Transition arity does not match number of tapes");
        }
        for (char symbol : transition.getReadSymbols()) {
            if (!tapeAlphabet_.count(symbol)) throw std::invalid_argument("Transition reads an invalid symbol");
        }
        for (char symbol : transition.getWriteSymbols()) {
            if (!tapeAlphabet_.count(symbol)) throw std::invalid_argument("Transition writes an invalid symbol");
        }
        for (char movement : transition.getMovements()) {
            if (movement != 'L' && movement != 'R' && movement != 'S') {
                throw std::invalid_argument("Transition contains an invalid movement");
            }
        }
        for (std::size_t j = i + 1; j < transitions_.size(); ++j) {
            if (transition.getCurrentState() == transitions_[j].getCurrentState() &&
                transition.getReadSymbols() == transitions_[j].getReadSymbols()) {
                throw std::invalid_argument("Machine is non-deterministic");
            }
        }
    }
}

/** @brief Resets the state and creates initialized tapes for an input. */
void TuringMachine::initializeTapes(const std::string& input) {
    currentState_ = initialState_;
    tapes_.assign(numberOfTapes_, Tape(blankSymbol_));
    tapes_[0].initialize(input);
    for (std::size_t i = 1; i < tapes_.size(); ++i) tapes_[i].initialize("");
}

/** @brief Finds the unique transition matching the current configuration. */
std::optional<Transition> TuringMachine::findApplicableTransition() const {
    for (const auto& transition : transitions_) {
        if (transition.getCurrentState() != currentState_) continue;
        bool matches = true;
        for (std::size_t i = 0; i < numberOfTapes_; ++i) {
            if (tapes_[i].read() != transition.getReadSymbols()[i]) {
                matches = false;
                break;
            }
        }
        if (matches) return transition;
    }
    return std::nullopt;
}

/** @brief Writes, moves heads, and changes the current state atomically. */
void TuringMachine::executeTransition(const Transition& transition) {
    for (std::size_t i = 0; i < numberOfTapes_; ++i) tapes_[i].write(transition.getWriteSymbols()[i]);
    for (std::size_t i = 0; i < numberOfTapes_; ++i) tapes_[i].move(transition.getMovements()[i]);
    currentState_ = transition.getNextState();
}

/** @brief Reports whether the current state is accepting. */
bool TuringMachine::isAccepting() const { return acceptingStates_.count(currentState_) != 0; }

/** @brief Executes until no transition applies and builds the result. */
SimulationResult TuringMachine::run(const std::string& input) {
    validate();
    initializeTapes(input);
    SimulationResult result;
    while (const auto transition = findApplicableTransition()) {
        executeTransition(*transition);
        ++result.steps;
    }
    result.accepted = isAccepting();
    result.finalState = currentState_;
    for (const auto& tape : tapes_) {
        result.tapeContents.push_back(tape.getVisibleContents());
        result.headPositions.push_back(tape.getHeadPosition());
    }
    return result;
}

/** @brief Returns the input alphabet used for input validation. */
const std::set<char>& TuringMachine::getInputAlphabet() const { return inputAlphabet_; }
/** @brief Returns the configured number of tapes. */
std::size_t TuringMachine::getNumberOfTapes() const { return numberOfTapes_; }
