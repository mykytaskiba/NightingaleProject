#include "pch.h"
#include "defines.h"

#include "gl_shader.h"
#include "gl_shader_uniform.h"
#include <GL/glew.h>


void GL_Shader::releaseShader()
{
   glDeleteShader(m_shaderHandle);
}


bool GL_Shader::compileShader(const char* vertexShaderSource, const char* fragmentShaderSource, GL_Shader& outShader)
{
    GL_Shader vertexShader;
    GL_Shader fragmentShader;
    if (!compileShader(ShaderType::VERTEX_SHADER, vertexShaderSource, vertexShader)) {
        return false;
    }
    if (!compileShader(ShaderType::FRAGMENT_SHADER, fragmentShaderSource, fragmentShader)) {
        return false;
    }

    uint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader.getHandle());
    glAttachShader(shaderProgram, fragmentShader.getHandle());
    glLinkProgram(shaderProgram);

    if (!verifyShaderSuccess(shaderProgram, GL_LINK_STATUS)) {
        return false;
    }

    fragmentShader.releaseShader();
    vertexShader.releaseShader();

    outShader.m_shaderHandle = shaderProgram;
    return true;
}

GL_Shader::GL_Shader() : m_shaderHandle(GL_INVALID_INDEX) {}

bool GL_Shader::isValid()
{
    return m_shaderHandle == GL_INVALID_INDEX;
}

void GL_Shader::activate()
{
    glUseProgram(m_shaderHandle);
}

uint GL_Shader::shaderTypeToGL(ShaderType shaderType)
{
    switch (shaderType) {
    case ShaderType::VERTEX_SHADER: return GL_VERTEX_SHADER;
    case ShaderType::FRAGMENT_SHADER: return GL_FRAGMENT_SHADER;
    case ShaderType::COMPUTE_SHADER: return GL_COMPUTE_SHADER;
    }
}

bool GL_Shader::verifyShaderSuccess(uint shaderID, uint operationType)
{
    //operationType = GL_COMPILE_STATUS for shader compilation
    //operationType = GL_LINK_STATUS for shader linking
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, operationType, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        assert(0);
        return false;
    }
    return true;
}

bool GL_Shader::compileShader(ShaderType type, const char* source, GL_Shader& outShader)
{

    uint shaderID;
    shaderID = glCreateShader(shaderTypeToGL(type));

    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);
    if (!verifyShaderSuccess(shaderID, GL_COMPILE_STATUS)) {
        return false;
    }

    outShader.m_shaderHandle = shaderID;
    return true;
}
