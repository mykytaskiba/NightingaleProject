#pragma once

#include "defines.h"

class ShaderSource {
private:
    string m_source;
    
    ShaderSource(char*);

public:

    ShaderSource() {}

    string getSource() { return m_source; }

    static ShaderSource fromFile(string const&);

};