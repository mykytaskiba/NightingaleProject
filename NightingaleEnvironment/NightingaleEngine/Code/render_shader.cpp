#include "pch.h"

#include "render_shader.h"
#include "shader_source.h"
#include "gl_shader_uniform.h"
#include "ngmath.h"


void RenderShader::activate()
{
    m_baseShader.activate();

}

void RenderShader::transferByName(string const& name, Matrix4x4 const& matrix)
{
    uint loc = glGetUniformLocation(m_baseShader.getHandle(), name.c_str());
    glUniformMatrix4fv(loc, 1, GL_TRUE, (float const*) &matrix(0,0));
}

void RenderShader::transferByName(string const& name, Color const& color)
{
    uint loc = glGetUniformLocation(m_baseShader.getHandle(), name.c_str());
    glUniform4fv(loc, 1, (float*) &color);
}

void RenderShader::transferByName(string const& name, Vector3 const& vec3)
{
    uint loc = glGetUniformLocation(m_baseShader.getHandle(), name.c_str());
    glUniform3fv(loc, 1, (float*)&vec3);
}

void RenderShader::transferArray(string const& name, vector<Matrix4x4> const& matricies)
{
    uint loc = glGetUniformLocation(m_baseShader.getHandle(), name.c_str());
    glUniformMatrix4fv(loc, matricies.size(), GL_TRUE, (float const*)matricies.data());
}

