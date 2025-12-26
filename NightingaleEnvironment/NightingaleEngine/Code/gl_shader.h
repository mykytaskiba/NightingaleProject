#pragma once

#include "defines.h"

class GL_Shader {

public:

    uint getHandle() const { return m_shaderHandle; }
    
    static bool compileShader(const char* vertexShader, const char* fragmentShader, GL_Shader& outShader);

    GL_Shader() : m_shaderHandle(GL_INVALID_INDEX) {}

    bool isValid() { return m_shaderHandle == GL_INVALID_INDEX; }

    void activate() { glUseProgram(m_shaderHandle); }

private:

    uint m_shaderHandle;

    enum class ShaderType {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        COMPUTE_SHADER,
    };


    static uint shaderTypeToGL(ShaderType shaderType) {
        switch (shaderType) {
        case ShaderType::VERTEX_SHADER: return GL_VERTEX_SHADER;
        case ShaderType::FRAGMENT_SHADER: return GL_FRAGMENT_SHADER;
        case ShaderType::COMPUTE_SHADER: return GL_COMPUTE_SHADER;
        }
    }
    static bool verifyShaderSuccess(uint shaderID, uint operationType);
    static bool compileShader(ShaderType type, const char* source, GL_Shader& outShader);

    
    void releaseShader();




};