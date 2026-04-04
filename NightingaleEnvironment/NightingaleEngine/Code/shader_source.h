#pragma once

#include "defines.h"

class ShaderSource {
private:
    std::string m_source;
    
    ShaderSource(char*);

public:

    ShaderSource() {}

    std::string getSource() { return m_source; }

    static ShaderSource fromFile(std::string const&);

};