#include "render_primitives.h"
#include "nightingale_assert.h"
#include "GL/glew.h"
#include "graphics_context.h"
#include "render_shader.h"
#include "mesh.h"
#include "transform.h"
#include "vector.h"
#include "ngmath.h"
#include "camera.h"

RenderPrimitives::RenderPrimitives(GraphicsContext& context) : m_context(context) {}

void RenderPrimitives::setLineShader(RenderShader* pLineShader)
{
    assert(m_pLineShader == nullptr);
    assert(pLineShader != nullptr);
    m_pLineShader = pLineShader;

    float interpolants[] = {
        0.0f, //Start point interpolant
        1.0f, //End point interpolant
    };

    glGenVertexArrays(1, &m_lineVAO);
    glBindVertexArray(m_lineVAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(interpolants), interpolants, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, 1 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
}

void RenderPrimitives::setUnlitShader(RenderShader* pUnlitShader)
{
    assert(m_pUnlitShader == nullptr);
    assert(pUnlitShader != nullptr);
    m_pUnlitShader = pUnlitShader;
}

void RenderPrimitives::setSphereMesh(Mesh* pSphere)
{
    assert(m_pSphere == nullptr);
    assert(pSphere != nullptr);
    m_pSphere = pSphere;
}

void RenderPrimitives::setCubeMesh(Mesh* pCube)
{
    assert(m_pCube == nullptr);
    assert(pCube != nullptr);
    m_pCube = pCube;
}

void RenderPrimitives::drawAxisAlignedBox(AxisAlignedBox const& box, Color const& color) const
{
    Vector3 min = box.min();
    Vector3 max = box.max();
    Vector3 extends = box.extents();
    Vector3 right = { extends[0],0,0 };
    Vector3 up = { 0,extends[1],0 };
    Vector3 forward = { 0,0,extends[2] };

    drawLine(min, min + up, color);
    drawLine(min + right, min + up + right, color);
    drawLine(min + forward, min + up + forward, color);
    drawLine(max, max - up, color);

    drawLine(min, min + right, color);
    drawLine(min, min + forward, color);
    drawLine(max - up, max - up - forward, color);
    drawLine(max - up, max - up - right, color);

    drawLine(max, max - right, color);
    drawLine(max, max - forward, color);
    drawLine(min + up, min + up + right, color);
    drawLine(min + up, min + up + forward, color);

}

void RenderPrimitives::drawLine(Vector3 const& from, Vector3 const& to, Color const& color) const
{
    m_context.setCurrentShader(m_pLineShader);

    if (m_context.updateDirtyState()) {
        m_pLineShader->transferByName("uFromPoint", from);
        m_pLineShader->transferByName("uDestPoint", to);

        m_pLineShader->transferByName("uColor", color);

        glBindVertexArray(m_lineVAO);
        glDrawArrays(GL_LINES, 0, 2);
    }
}

void RenderPrimitives::drawSphere(Vector3 const& at, float scale, Color const& color) const
{
    m_context.setCurrentShader(m_pUnlitShader);

    Transform transform;
    transform.scale = Vector3(scale, scale, scale);
    transform.position = at;

    m_pUnlitShader->transferByName("uModelMatrix", transform.getMatrix());
    m_pUnlitShader->transferByName("uColor", color);
    m_context.draw(m_pSphere);

}

void RenderPrimitives::drawCube(Vector3 const& center, Vector3 const& scale, Color const& color) const
{
    m_context.setCurrentShader(m_pUnlitShader);

    Transform transform;
    transform.position = center;

    Matrix4x4 scaleMatrix;
    scaleMatrix.make_scale(scale);
    Matrix4x4 transformMatrix = transform.getMatrix() * scaleMatrix;

    m_pUnlitShader->transferByName("uModelMatrix", transformMatrix);
    m_pUnlitShader->transferByName("uColor", color);
    m_context.draw(m_pCube);
}


void RenderPrimitives::drawWireSphere(Vector3 const& at, float scale, Color const& color) const
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    drawSphere(at, scale, color);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void RenderPrimitives::drawWireCube(Vector3 const& center, Vector3 const& scale, Color const& color) const
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    drawCube(center, scale, color);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}