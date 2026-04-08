#include "graphics_context.h"
#include "ngmath.h"
#include <GL/glew.h>
#include "render_shader.h"
#include "camera.h"

void GraphicsContext::setCamera(Camera* pCamera)
{
    if (m_pCamera == pCamera) return;

    m_pCamera = pCamera;
    m_bDirtyCamera = true;
}

void GraphicsContext::setCurrentShader(RenderShader* pShader)
{
    if (m_pCurrentShader == pShader) return;

    m_pCurrentShader = pShader;
    m_bDirtyShader = true;

    m_pCurrentShader->activate();
    if (m_pCamera != nullptr) {
        m_pCurrentShader->transferByName("uViewMatrix", m_pCamera->GetViewMatrix());
        m_pCurrentShader->transferByName("uProjMatrix", m_pCamera->GetPerspectiveMatrix());
    }
}

RenderShader* GraphicsContext::getCurrentShader()
{
    return m_pCurrentShader;
}


void GraphicsContext::beginFrame()
{
    m_bDirtyCamera = true;
}

void GraphicsContext::endFrame()
{
    m_pCurrentShader = nullptr;
}
