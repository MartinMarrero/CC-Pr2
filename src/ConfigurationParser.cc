#include "ConfigurationParser.h"

#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace {
/** @brief Removes whitespace from both ends of a string. */
std::string trim(std::string value) {
    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front()))) value.erase(value.begin());
    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back()))) value.pop_back();
    return value;
}

/** @brief Splits a string and trims every resulting field. */
std::vector<std::string> split(const std::string& value, char delimiter) {
    std::vector<std::string> parts;
    std::string part;
    std::istringstream stream(value);
    while (std::getline(stream, part, delimiter)) parts.push_back(trim(part));
    return parts;
}

/** @brief Parses a comma-separated alphabet of single-character symbols. */
std::set<char> parseAlphabet(const std::string& value) {
    std::set<char> result;
    for (const auto& item : split(value, ',')) {
        if (item.size() != 1) throw std::invalid_argument("Alphabet symbols must be single characters");
        result.insert(item[0]);
    }
    return result;
}

/** @brief Parses comma-separated transition symbols. */
std::vector<char> parseSymbols(const std::string& value) {
    std::vector<char> result;
    for (const auto& item : split(value, ',')) {
        if (item.size() != 1) throw std::invalid_argument("Transition symbols must be single characters");
        result.push_back(item[0]);
    }
    return result;
}

/** @brief Parses whitespace-separated words. */
std::vector<std::string> parseWords(const std::string& value) {
    std::vector<std::string> result;
    std::istringstream stream(value);
    std::string word;
    while (stream >> word) result.push_back(word);
    return result;
}

/** @brief Parses a whitespace-separated legacy alphabet. */
std::set<char> parseLegacyAlphabet(const std::string& value) {
    std::set<char> result;
    for (const auto& item : parseWords(value)) {
        if (item.size() != 1) throw std::invalid_argument("Alphabet symbols must be single characters");
        result.insert(item[0]);
    }
    return result;
}

/** @brief Reads non-empty, non-comment configuration lines. */
std::vector<std::string> readMeaningfulLines(std::ifstream& file) {
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (!line.empty() && line.front() != '#') lines.push_back(line);
    }
    return lines;
}

/** @brief Builds a machine from the positional practice-file format. */
TuringMachine parseLegacyFormat(const std::vector<std::string>& lines) {
    if (lines.size() < 8) {
        throw std::invalid_argument("Legacy configuration requires at least eight data lines");
    }

    TuringMachine machine;
    std::set<std::string> states;
    for (const auto& state : parseWords(lines[0])) states.insert(state);
    machine.setStates(states);
    machine.setInputAlphabet(parseLegacyAlphabet(lines[1]));
    machine.setTapeAlphabet(parseLegacyAlphabet(lines[2]));
    machine.setInitialState(lines[3]);
    if (lines[4].size() != 1) throw std::invalid_argument("Blank symbol must be one character");
    machine.setBlankSymbol(lines[4][0]);
    for (const auto& state : parseWords(lines[5])) machine.addAcceptingState(state);
    std::istringstream tapeCount(lines[6]);
    std::size_t numberOfTapes = 0;
    if (!(tapeCount >> numberOfTapes)) throw std::invalid_argument("Invalid number of tapes");
    machine.setNumberOfTapes(numberOfTapes);

    for (std::size_t i = 7; i < lines.size(); ++i) {
        std::istringstream transitionLine(lines[i]);
        std::string currentState;
        std::string readSymbols;
        std::string nextState;
        std::string writeSymbols;
        std::string movements;
        if (!(transitionLine >> currentState >> readSymbols >> nextState >> writeSymbols >> movements)) {
            throw std::invalid_argument("Invalid transition on legacy configuration line " +
                                        std::to_string(i + 1));
        }
        machine.addTransition(Transition(currentState, parseSymbols(readSymbols), nextState,
                                         parseSymbols(writeSymbols), parseSymbols(movements)));
    }
    machine.validate();
    return machine;
}
}

/** @brief Loads either supported configuration format and validates it. */
TuringMachine ConfigurationParser::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("Cannot open configuration file: " + filename);

    const auto lines = readMeaningfulLines(file);
    if (lines.empty()) throw std::invalid_argument("Configuration file is empty");
    if (lines.front().find('=') == std::string::npos) return parseLegacyFormat(lines);

    TuringMachine machine;
    std::string line;
    file.clear();
    file.seekg(0);
    std::size_t lineNumber = 0;
    while (std::getline(file, line)) {
        ++lineNumber;
        line = trim(line);
        if (line.empty() || line.front() == '#') continue;
        const auto separator = line.find('=');
        if (separator == std::string::npos) {
            throw std::invalid_argument("Invalid configuration line " + std::to_string(lineNumber));
        }
        const std::string key = trim(line.substr(0, separator));
        const std::string value = trim(line.substr(separator + 1));
        try {
            if (key == "states") {
                std::set<std::string> states;
                for (const auto& state : split(value, ',')) states.insert(state);
                machine.setStates(states);
            } else if (key == "input_alphabet") machine.setInputAlphabet(parseAlphabet(value));
            else if (key == "tape_alphabet") machine.setTapeAlphabet(parseAlphabet(value));
            else if (key == "initial_state") machine.setInitialState(value);
            else if (key == "blank") {
                if (value.size() != 1) throw std::invalid_argument("blank must be one character");
                machine.setBlankSymbol(value[0]);
            } else if (key == "accepting_states") {
                for (const auto& state : split(value, ',')) machine.addAcceptingState(state);
            } else if (key == "tapes") {
                machine.setNumberOfTapes(static_cast<std::size_t>(std::stoul(value)));
            } else if (key == "transition") {
                const auto fields = split(value, '|');
                if (fields.size() != 5) throw std::invalid_argument("transition requires five fields");
                machine.addTransition(Transition(fields[0], parseSymbols(fields[1]), fields[2],
                                                 parseSymbols(fields[3]), parseSymbols(fields[4])));
            } else {
                throw std::invalid_argument("Unknown configuration key: " + key);
            }
        } catch (const std::exception& error) {
            throw std::invalid_argument("Configuration line " + std::to_string(lineNumber) + ": " + error.what());
        }
    }
    machine.validate();
    return machine;
}
