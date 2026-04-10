#include "ngmath.h"
#include "render_shader.h"
#include <GL/glew.h>
#include "color.h"


void RenderShader::activate()
{
    m_baseShader.activate();

}

void RenderShader::transferByName(std::string const& name, Matrix4x4 const& matrix)
{
    uint loc = glGetUniformLocation(m_baseShader.getHandle(), name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, getMatrixData(matrix));
}

void RenderShader::transferByName(std::string const& name, Color const& color)
{
    uint loc = glGetUniformLocation(m_baseShader.getHandle(), name.c_str());
    glUniform4fv(loc, 1, (float*) &color);
}

void RenderShader::transferByName(std::string const& name, Vector3 const& vec3)
{
    uint loc = glGetUniformLocation(m_baseShader.getHandle(), name.c_str());
    glUniform3fv(loc, 1, (float*)&vec3);
}

void RenderShader::transferArray(std::string const& name, std::vector<Matrix4x4> const& matricies)
{
    uint loc = glGetUniformLocation(m_baseShader.getHandle(), name.c_str());
    glUniformMatrix4fv(loc, matricies.size(), GL_FALSE, (float const*)matricies.data());
}

float const* RenderShader::getMatrixData(Matrix4x4 const& matrix)
{
    return (float const*)&matrix;
}

