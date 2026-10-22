#include "CommandLineParser.h"

#include <stdexcept>

/** @brief Parses and validates command-line options. */
void CommandLineParser::parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        const std::string argument(argv[i]);
        if (argument == "-config" || argument == "--config") {
            if (++i >= argc) throw std::invalid_argument("-config requires a file path");
            configFile_ = argv[i];
        } else if (argument == "-in" || argument == "--in") {
            if (++i >= argc) throw std::invalid_argument("-in requires a file path");
            inputFile_ = argv[i];
        } else if (argument == "-h" || argument == "--help") {
            throw std::invalid_argument("Usage: turing_simulator -config FILE [-in FILE]");
        } else {
            throw std::invalid_argument("Unknown argument: " + argument);
        }
    }
    if (configFile_.empty()) throw std::invalid_argument("The -config argument is required");
}

/** @brief Returns the configuration path. */
const std::string& CommandLineParser::getConfigFile() const { return configFile_; }
/** @brief Returns the optional input path. */
const std::string& CommandLineParser::getInputFile() const { return inputFile_; }
