#include "pch.h"
#include "engine_functions.h"
#include "game_object.h"
#include "engine_internals.h"
#include "render_node.h"
#include "asset_manager.h"

void EngineFunctions::Setup(GameObject* pGameObject)
{
    GameObject* pParent = &EngineInternals::Scene().m_root;
    assert(pGameObject != nullptr);
    assert(pParent != nullptr);

    SetParent(pGameObject, pParent);

    pGameObject->init();

    //This could return null, in this case this gameObject is not renderable
    RenderNode* pRenderNode = pGameObject->createRenderNode();
    if (pRenderNode != nullptr) {
        AssignRenderNode(pGameObject, pRenderNode);
    }


}

void EngineFunctions::PreSetup(GameObject* setup)
{
    SceneChangePackage package;
    package.pGameObject = setup;
    EngineInternals::Scene().addPackage(package);
}

void EngineFunctions::SetParent(GameObject* pGameObject, GameObject* pParent)
{
    assert(pGameObject != nullptr);
    assert(pParent != nullptr);

    assert(pGameObject->m_pParent == nullptr);

    pGameObject->m_pParent = pParent;
    pParent->m_children.push_back(pGameObject);
}

void EngineFunctions::AssignRenderNode(GameObject* pGameObject, RenderNode* pRenderNode)
{
    assert(pGameObject != nullptr);
    assert(pRenderNode != nullptr);

    pGameObject->m_pRenderNode = pRenderNode;
    pRenderNode->init(pGameObject);

    EngineInternals::Renderer().registerRenderable(pRenderNode);
}

Camera& EngineFunctions::Camera()
{
    return EngineInternals::Camera();
}


int EngineFunctions::ScreenWidth()
{
    return EngineInternals::Renderer().m_screen.Width;
}

int EngineFunctions::ScreenHeight()
{
    return EngineInternals::Renderer().m_screen.Height;
}

time_type EngineFunctions::DeltaTime()
{
    return EngineInternals::pEngine->m_frameController.getDeltaTime();
}

