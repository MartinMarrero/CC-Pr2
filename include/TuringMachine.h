#pragma once

#include "SimulationResult.h"
#include "Tape.h"
#include "Transition.h"

#include <optional>
#include <set>
#include <string>
#include <vector>

class TuringMachine {
public:
    void addState(const std::string& state);
    void setStates(const std::set<std::string>& states);
    void setInputAlphabet(const std::set<char>& alphabet);
    void setTapeAlphabet(const std::set<char>& alphabet);
    void setInitialState(const std::string& state);
    void setBlankSymbol(char symbol);
    void addAcceptingState(const std::string& state);
    void setNumberOfTapes(std::size_t numberOfTapes);
    void addTransition(const Transition& transition);

    void validate() const;
    void initializeTapes(const std::string& input);
    std::optional<Transition> findApplicableTransition() const;
    void executeTransition(const Transition& transition);
    bool isAccepting() const;
    SimulationResult run(const std::string& input);

    const std::set<char>& getInputAlphabet() const;
    std::size_t getNumberOfTapes() const;

private:
    std::set<std::string> states_;
    std::set<char> inputAlphabet_;
    std::set<char> tapeAlphabet_;
    std::string initialState_;
    std::string currentState_;
    char blankSymbol_ = '_';
    std::set<std::string> acceptingStates_;
    std::size_t numberOfTapes_ = 1;
    std::vector<Transition> transitions_;
    std::vector<Tape> tapes_;
};
