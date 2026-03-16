#pragma once
#include "defines.h"

class ArgumentHelpers {
public:
    static std::string getNextRaw(std::string&);
    static std::string getNextWithDefines(std::string&);

    static std::vector<std::string> separateByDelimiter(std::string const& inString, char delimiter);

};