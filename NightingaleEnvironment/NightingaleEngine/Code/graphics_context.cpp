#include "pch.h"
#include "graphics_context.h"
#include "render_shader.h"
#include "engine_internals.h"
#include "mesh.h"
#include "ngmath.h"

void GraphicsContext::setCurrentShader(RenderShader* pShader)
{
    if (m_pCurrentShader == pShader) return;

    m_pCurrentShader = pShader;

    m_pCurrentShader->activate();

    m_pCurrentShader->transferByName("uViewMatrix", EngineInternals::Camera().GetViewMatrix());
    m_pCurrentShader->transferByName("uProjMatrix", EngineInternals::Camera().GetPerspectiveMatrix());
}

RenderShader* GraphicsContext::getCurrentShader()
{
    return m_pCurrentShader;
}

void GraphicsContext::init()
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

void GraphicsContext::drawAxisAlignedBox(AxisAlignedBox const& box, Color const& color)
{
    Vector3 min = box.min();
    Vector3 max = box.max();
    Vector3 extends = box.extends();
    Vector3 right = { extends[0],0,0 };
    Vector3 up = { 0,extends[1],0 };
    Vector3 forward = { 0,0,extends[2] };

    drawLine(min, min + up);
    drawLine(min + right, min + up + right);
    drawLine(min + forward, min + up + forward);
    drawLine(max, max - up);

    drawLine(min, min + right);
    drawLine(min, min + forward);
    drawLine(max - up, max - up - forward);
    drawLine(max - up, max - up - right);

    drawLine(max, max - right);
    drawLine(max, max - forward);
    drawLine(min + up, min + up + right);
    drawLine(min + up, min + up + forward);

}

void GraphicsContext::drawLine(Vector3 const& from, Vector3 const& to, Color const& color)
{
    setCurrentShader(m_pLineShader);    
    
    m_pLineShader->transferByName("uFromPoint", from);
    m_pLineShader->transferByName("uDestPoint", to);

    m_pLineShader->transferByName("uColor", color);

    glBindVertexArray(m_lineVAO);
    glDrawArrays(GL_LINES, 0, 2); 
}

void GraphicsContext::drawSphere(Vector3 const& at, float scale, Color const& color)
{
    setCurrentShader(m_pUnlitShader);

    Transform transform;
    transform.scale = scale;
    transform.position = at;

    m_pUnlitShader->transferByName("uModelMatrix", transform.getMatrix());
    m_pUnlitShader->transferByName("uColor", color);
    m_pSphere->Draw();

}

void GraphicsContext::drawCube(Vector3 const& center, Vector3 const& scale, Color const& color)
{
    setCurrentShader(m_pUnlitShader);

    Transform transform;
    transform.scale = 1.0f;
    transform.position = center;

    Matrix4x4 scaleMatrix;
    scaleMatrix.make_scale(scale);
    Matrix4x4 transformMatrix = transform.getMatrix() * scaleMatrix;

    m_pUnlitShader->transferByName("uModelMatrix", transformMatrix);
    m_pUnlitShader->transferByName("uColor", color);
    m_pCube->Draw();
}

void GraphicsContext::drawWireSphere(Vector3 const& at, float scale, Color const& color)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    drawSphere(at, scale, color);
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GraphicsContext::drawWireCube(Vector3 const& center, Vector3 const& scale, Color const& color)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDisable(GL_CULL_FACE);
    drawCube(center, scale, color); 
    glEnable(GL_CULL_FACE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GraphicsContext::endFrame()
{
    m_pCurrentShader = nullptr;
}
