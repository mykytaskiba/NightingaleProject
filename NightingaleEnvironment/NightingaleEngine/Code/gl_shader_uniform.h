#pragma once
#include "defines.h"

class GL_Shader;

class GL_Shader_Uniform {

private:
    static constexpr uint MAX_NAME_LENGTH = 256; // maximum name for each variable length

    char m_variableName;
    uint m_variableLocation;
    GLint m_size;
    GLenum m_type;

    //constructor for a GL_Shader_Uniform.
    GL_Shader_Uniform(uint shaderHandle, uint index);

    //no param constructor
    GL_Shader_Uniform() : m_variableLocation(-1), m_size(0), m_type(0) {}
   
public:
    static vector<GL_Shader_Uniform> makeUniformList(GL_Shader const& shader);

};