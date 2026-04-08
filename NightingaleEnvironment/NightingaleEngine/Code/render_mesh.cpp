#include "pch.h"
#include "render_mesh.h"
#include "game_object.h"
#include "mesh.h"
#include "asset_manager.h"
#include "ngrender.h"


void RenderMeshNode::render(GraphicsContext& context)
{
    assert(m_pMesh != nullptr);
    m_pShader = AssetManager<RenderShader>::retrieve("main_shader");
    assert(m_pShader != nullptr);

    context.setCurrentShader(m_pShader);
    m_pShader->transferByName("uModelMatrix", m_transform.getMatrix());
    m_pShader->transferByName("uColor", m_color);
    m_pMesh->Draw();
}

void RenderMeshNode::setMesh(Mesh* pMesh)
{
    m_pMesh = pMesh;
}

