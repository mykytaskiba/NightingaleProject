#pragma once

#include "gl_shader.h"
#include "gl_shader_uniform.h"
#include "color.h"
#include "ngmath.h"

//Shader that is used for rendering (Default vertex -> fragment pipeline)
class RenderShader {
public:

    GL_Shader m_baseShader;
    vector<GL_Shader_Uniform> m_uniformList;


public:

    RenderShader(GL_Shader baseShader) : m_baseShader(baseShader) {}

    void activate();
    void transferByName(string const& name, Matrix4x4 const& matrix);
    void transferByName(string const& name, Color const& color);
    void transferByName(string const& name, Vector3 const& vec3);

    void transferArray(string const& name, vector<Matrix4x4> const& matricies);


};