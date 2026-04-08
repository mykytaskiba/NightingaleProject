#pragma once
#include "ngmath.h"
#include <string>
#include "gl_shader.h"

//Shader that is used for rendering (Default vertex -> fragment pipeline)
class RenderShader {
public:

    GL_Shader m_baseShader;

public:

    RenderShader(GL_Shader baseShader) : m_baseShader(baseShader) {}

    void activate();
    void transferByName(std::string const& name, Matrix4x4 const& matrix);
    void transferByName(std::string const& name, Color const& color);
    void transferByName(std::string const& name, Vector3 const& vec3);

    void transferArray(std::string const& name, std::vector<Matrix4x4> const& matricies);

    float const* getMatrixData(Matrix4x4 const& matrix);



};