#pragma once
#include <vector>

class GL_Shader;

class GL_Shader_Uniform {

private:
    static constexpr unsigned int MAX_NAME_LENGTH = 256; // maximum name for each variable length

    char m_variableName;
    unsigned int m_variableLocation;
    int m_size; //TO DO: THIS IS REALLY A GLint
    unsigned int m_type; //TO DO: THIS IS REALLY AN ENUM

    //constructor for a GL_Shader_Uniform.
    GL_Shader_Uniform(unsigned int shaderHandle, unsigned int index);

    //no param constructor
    GL_Shader_Uniform() : m_variableLocation(-1), m_size(0), m_type(0) {}
   
public:
    static std::vector<GL_Shader_Uniform> makeUniformList(GL_Shader const& shader);

};