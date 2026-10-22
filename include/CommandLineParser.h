#pragma once

#include <string>

class CommandLineParser {
public:
    void parse(int argc, char* argv[]);
    const std::string& getConfigFile() const;
    const std::string& getInputFile() const;

private:
    std::string configFile_;
    std::string inputFile_;
};
