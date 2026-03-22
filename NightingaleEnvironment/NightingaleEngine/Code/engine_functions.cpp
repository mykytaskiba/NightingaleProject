#include "pch.h"
#include "engine_functions.h"
#include "game_object.h"
#include "engine_internals.h"
#include "render_node.h"
#include "asset_manager.h"

void EngineFunctions::Setup(GameObject* pGameObject, GameObject* pParent)
{
    assert(pGameObject != nullptr);
    assert(pParent != nullptr);

    SetParent(pGameObject, pParent);

    pGameObject->init();
}

void EngineFunctions::PreSetup(GameObject* pGameObject, GameObject* pParent)
{
    if (pParent == nullptr) {
        pParent = &EngineInternals::Scene().m_root;
    }

    scene().addDeferredFunction([pGameObject, pParent] {
            EngineFunctions::Setup(pGameObject,pParent);
        }
    );
}

void EngineFunctions::SetParent(GameObject* pGameObject, GameObject* pParent)
{
    assert(pGameObject != nullptr);
    assert(pParent != nullptr);

    assert(pGameObject->m_pParent == nullptr);

    pGameObject->m_pParent = pParent;
    pParent->m_vChildren.push_back(pGameObject);
}

void EngineFunctions::AssignRenderNode(GameObject* pGameObject, RenderNode* pRenderNode)
{
    assert(pGameObject != nullptr);
    assert(pRenderNode != nullptr);

    pGameObject->m_pRenderNode = pRenderNode;
    pRenderNode->init(pGameObject);

    EngineInternals::Renderer().registerRenderable(pRenderNode);
}

void EngineFunctions::AttachPhysicsBody(GameObject* pGameObject)
{
    assert(pGameObject != nullptr);

    PhysicsBody* pBody = new PhysicsBody();
    physics().addBody(pBody);
    pGameObject->m_pPhysicsBody = pBody;

    pBody->update(0.0f);
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

TTimeDelta EngineFunctions::delta_time()
{
    return EngineInternals::pEngine->m_frameController.getDeltaTime();
}

ExecutionResult EngineFunctions::ExecuteCommand(string const& command)
{
    return EngineInternals::pEngine->m_scriptEnvironment.execute(command);
}

void EngineFunctions::SetWindowTitle(string const& title)
{
    EngineInternals::pEngine->m_window.setTitle(title); 
}

void EngineFunctions::SetTargetFramerate(int framerate)
{
    EngineInternals::pEngine->m_frameController.setTargetFrameRate(framerate);
}

float EngineFunctions::getTargetFramerate()
{
    //TO DO NULL PTR ASSERT!
    return EngineInternals::pEngine->m_frameController.getTargetFrameRate();
}

uint EngineFunctions::getTargetFramerateInt()
{
    return (uint)getTargetFramerate();
}

CallbackHandler& EngineFunctions::getUpdateCallback()
{
    assert(EngineInternals::pEngine != nullptr);
    if (EngineInternals::pEngine == nullptr) {
        //TO DO: return a dummy callback handler to prevent nullptr
    }

    return EngineInternals::pEngine->m_updateCallback;
}

Renderer& EngineFunctions::Renderer()
{
    return EngineInternals::pEngine->m_renderer;
}

Physics& EngineFunctions::physics()
{
    return EngineInternals::pEngine->m_physics;
}

Scene& EngineFunctions::scene()
{
    return EngineInternals::pEngine->m_scene;
}

ExecutionState& EngineFunctions::execution_state()
{
    return EngineInternals::pEngine->m_scriptEnvironment.getExecutionState();
}

HotkeyManager& EngineFunctions::hotkeyManager()
{
    return EngineInternals::pEngine->m_hotkeyManager;
}

void EngineFunctions::close_app()
{
    EngineInternals::pEngine->m_bRunning = false;
}

Factory<std::string, GameObject> const& EngineFunctions::factoryGameObject()
{
    return EngineInternals::pEngine->m_factoryGameObject;
}

