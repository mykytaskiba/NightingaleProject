#pragma once

#include "defines.h"

class GL_Shader {

public:

    uint getHandle() const { return m_shaderHandle; }
    
    static bool compileShader(const char* vertexShader, const char* fragmentShader, GL_Shader& outShader);

    GL_Shader();

    bool isValid();

    void activate();

private:

    uint m_shaderHandle;

    enum class ShaderType {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        COMPUTE_SHADER,
    };


    static uint shaderTypeToGL(ShaderType shaderType);
    static bool verifyShaderSuccess(uint shaderID, uint operationType);
    static bool compileShader(ShaderType type, const char* source, GL_Shader& outShader);

    
    void releaseShader();




};