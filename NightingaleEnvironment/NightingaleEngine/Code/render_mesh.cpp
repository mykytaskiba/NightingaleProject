#include "pch.h"
#include "render_mesh.h"
#include "render_shader.h"
#include "game_object.h"
#include "mesh.h"
#include "asset_manager.h"
#include "graphics_context.h"

void RenderMeshNode::init(GameObject* pGameObject)
{
    m_pGameObject = pGameObject;

    m_pShader = AssetManager<RenderShader>::retrieve("main_shader");
    assert(m_pShader != nullptr);
}

void RenderMeshNode::render(GraphicsContext& context)
{
    assert(m_pGameObject != nullptr);
    assert(m_pMesh != nullptr);
    assert(m_pShader != nullptr);

    context.setCurrentShader(m_pShader);
    m_pShader->transferByName("uModelMatrix", m_pGameObject->getTransform().getMatrix());
    m_pShader->transferByName("uColor", m_color);
    m_pMesh->Draw();
}

void RenderMeshNode::setMesh(Mesh* pMesh)
{
    m_pMesh = pMesh;
}

void RenderMeshNode::setColor(Color const& color)
{
    m_color = color;
}
