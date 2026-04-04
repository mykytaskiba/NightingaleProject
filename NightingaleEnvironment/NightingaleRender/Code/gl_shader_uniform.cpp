#include "gl_shader_uniform.h"
#include "gl_shader.h"
#include <GL/glew.h>

GL_Shader_Uniform::GL_Shader_Uniform(unsigned int shaderHandle, unsigned int index) : m_variableLocation(-1), m_size(0), m_type(0)
{
    GLint length; //length of the name


    glGetActiveUniform(shaderHandle, index, MAX_NAME_LENGTH, &length, &m_size, &m_type, &m_variableName);
    m_variableLocation = glGetUniformLocation(shaderHandle, &m_variableName);
}

std::vector<GL_Shader_Uniform> GL_Shader_Uniform::makeUniformList(GL_Shader const& shader)
{
    std::vector<GL_Shader_Uniform> uniformList;

    unsigned int shaderHandle = shader.getHandle();

    GLint uniformCount;
    glGetProgramiv(shaderHandle, GL_ACTIVE_UNIFORMS, &uniformCount);
    printf("Active Uniforms: %d\n", uniformCount);

    uniformList.reserve(uniformCount);
    for (unsigned int i = 0; i < uniformCount; i++)
    {
        uniformList.push_back(GL_Shader_Uniform(shaderHandle,i));
    }
    return uniformList;
}

