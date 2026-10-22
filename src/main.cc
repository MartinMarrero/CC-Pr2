#include "CommandLineParser.h"
#include "ConfigurationParser.h"
#include "InputReader.h"
#include "OutputFormatter.h"

#include <exception>
#include <iostream>

/** @brief Runs the complete command-line application pipeline. */
int main(int argc, char* argv[]) {
    try {
        CommandLineParser arguments;
        arguments.parse(argc, argv);
        TuringMachine machine = ConfigurationParser::parseFile(arguments.getConfigFile());
        const auto inputs = arguments.getInputFile().empty()
                                ? InputReader::readFromConsole()
                                : InputReader::readFromFile(arguments.getInputFile());
        for (const auto& input : inputs) {
            InputReader::validateInput(input, machine);
            std::cout << "input: " << (input.empty() ? "<empty>" : input) << '\n';
            OutputFormatter::printResult(machine.run(input));
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
