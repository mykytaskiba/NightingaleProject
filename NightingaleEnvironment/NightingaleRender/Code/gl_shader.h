#pragma once

class GL_Shader {

public:

    unsigned int getHandle() const { return m_shaderHandle; }
    
    static bool compileShader(const char* vertexShader, const char* fragmentShader, GL_Shader& outShader);

    GL_Shader();

    bool isValid();

    void activate();

private:

    unsigned int m_shaderHandle;

    enum class ShaderType {
        VERTEX_SHADER,
        FRAGMENT_SHADER,
        COMPUTE_SHADER,
    };


    static unsigned int shaderTypeToGL(ShaderType shaderType);
    static bool verifyShaderSuccess(unsigned int shaderID, unsigned int operationType);
    static bool compileShader(ShaderType type, const char* source, GL_Shader& outShader);

    
    void releaseShader();




};