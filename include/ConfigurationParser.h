#pragma once

#include "TuringMachine.h"

#include <string>

class ConfigurationParser {
public:
    static TuringMachine parseFile(const std::string& filename);
};
