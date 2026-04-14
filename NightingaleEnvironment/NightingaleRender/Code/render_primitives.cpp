#include "render_primitives.h"
#include "asset_manager.h"

void RenderPrimitives::init()
{
    float interpolants[] = {
        0.0f, //Start point interpolant
        1.0f, //End point interpolant
    };

    m_lineVAO;
    glGenVertexArrays(1, &m_lineVAO);
    glBindVertexArray(m_lineVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(interpolants), interpolants, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    m_pLineShader = AssetManager<RenderShader>::retrieve("line_shader");
    assert(m_pLineShader != nullptr);

    m_pSphere = AssetManager<Mesh>::retrieve("sphere_mesh");
    assert(m_pSphere != nullptr);

    m_pCube = AssetManager<Mesh>::retrieve("cube_mesh");
    assert(m_pCube != nullptr);

    m_pUnlitShader = AssetManager<RenderShader>::retrieve("unlit_shader");
    assert(m_pUnlitShader != nullptr);
}
