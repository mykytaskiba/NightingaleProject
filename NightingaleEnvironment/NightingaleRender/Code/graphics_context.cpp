#include "graphics_context.h"
#include "ngmath.h"
#include <GL/glew.h>
#include "render_shader.h"
#include "camera.h"
#include "mesh.h"

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
}

RenderShader* GraphicsContext::getCurrentShader()
{
    return m_pCurrentShader;
}


void GraphicsContext::draw(Mesh* pMesh) {
    if (updateDirtyState()) {
        pMesh->Draw();
    }
}

//Check for any dirty state and update
bool GraphicsContext::updateDirtyState() {

    if (m_pCurrentShader == nullptr || m_pCamera == nullptr) {
        return false;
    }

    if (m_bDirtyCamera) {
        m_bDirtyCamera = false;
        m_pCamera->SetTargetSize(m_targetWidth, m_targetHeight);
        
        m_bDirtyShader = true; //a dirty camera means shader needs to be updated
    }

    if (m_bDirtyShader) {
        m_bDirtyShader = false;
        m_pCurrentShader->activate();

        m_pCurrentShader->transferByName("uViewMatrix", m_pCamera->GetViewMatrix());
        m_pCurrentShader->transferByName("uProjMatrix", m_pCamera->GetPerspectiveMatrix());
    }

    return true;
}

void GraphicsContext::beginFrame()
{
    m_bDirtyCamera = true;
}

void GraphicsContext::endFrame()
{
    m_pCurrentShader = nullptr;
}
