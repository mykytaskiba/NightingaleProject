#include "pch.h"

#include "engine.h"
#include "logger.h"

#include "engine_internals.h"
#include "console.h"
#include "engine_functions.h"



EngineSettings& Engine::settings()
{
    return m_settings;
}

void Engine::run()
{
    register_systems();
    
    m_bRunning = true;
    init();
    update();
    shutdown();
}

Engine::Engine()
{
    setDefaultSettings();
}

void Engine::register_systems()
{
    m_initCallback.addCallback({ 50, [this] {defaultGameObjectFactory(); } });
    m_initCallback.addCallback({ 74, [this] {m_window.init(); } });
    m_initCallback.addCallback({ 75, [this] {m_debugUI.init(m_window); } });
    m_initCallback.addCallback({ 80, [this] {m_input.init(); } });
    m_initCallback.addCallback({ 90, [this] {m_scene.init(); } });
    m_initCallback.addCallback({ 95, [this] {  loadCommands(); } });
    m_initCallback.addCallback({ 100, [this] {m_renderer.init(); } });
    m_initCallback.addCallback({ 110, [this] {CS550TempTestFuncInit(); } });
    
    m_updateCallback.addCallback({ 60, [this] {m_frameController.frameStart(); } });
    m_updateCallback.addCallback({ 70, [this] {m_debugUI.newFrame(); } });
    m_updateCallback.addCallback({ 80, [this] {m_input.captureInputState(); } });
    m_updateCallback.addCallback({ 81, [this] {m_hotkeyManager.processHotkeys(); } });
    m_updateCallback.addCallback({ 85, [this] {m_scene.syncObjectToPhysics(); } });
    m_updateCallback.addCallback({ 90, [this] {m_scene.tick(); } });
    m_updateCallback.addCallback({ 185, [this] {m_scene.syncPhysicsToObject(); } });
    m_updateCallback.addCallback({ 190, [this] {m_physics.update(m_frameController.getDeltaTime()); } });
    m_updateCallback.addCallback({ 200, [this] {m_renderer.render(); } });
    m_updateCallback.addCallback({ 230, [this] {m_debugUI.endFrame(); } });
    m_updateCallback.addCallback({ 240, [this] {m_window.update(); } });
    m_updateCallback.addCallback({ 250, [this] {m_frameController.frameEnd(); } });
    
    m_shutdownCallback.addCallback({ 100, [this] {m_debugUI.shutdown(); } });

}

void Engine::init()
{
    EngineInternals::init(*this);
    
    //Currently scripting environment is handled differently than the rest of system
    m_scriptEnvironment.init();
    registerConsoleCommands();

    m_initCallback.execute();
}

void Engine::update()
{
    while (m_bRunning)
    {
        m_updateCallback.execute();
    }
}

void Engine::shutdown()
{
    m_shutdownCallback.execute();
}

void Engine::registerConsoleCommands()
{
    for (CommandInterface*& command : m_settings.console_commands) {
        m_scriptEnvironment.registerSingleCommand(command);
    }
}

void Engine::loadCommands()
{
    for (std::string const command : m_settings.load_commands) {
        ExecutionResult result = m_scriptEnvironment.execute(command);
        assert(result.bSuccess);
    }
}

void Engine::setDefaultSettings()
{
    m_settings.load_commands.push_back(".execute data/core/load.ngs");

    defaultSettings_CoreCommands();
}

#include "render_mesh.h"
#include "physics_debug_render_pass.h"
#include "loader.h"
#include "file_source.h"
void Engine::CS550TempTestFuncInit()
{
    /*
    GameObject* pGameObject = EngineFunctions::InstantiateGameObject<GameObject>();
    RenderMeshNode* pRenderNode = new RenderMeshNode();
    pRenderNode->setMesh(AssetManager<Mesh>::retrieve("sphere_mesh"));

    EngineFunctions::AssignRenderNode(pGameObject, pRenderNode);
    */
    
    

    /*
    */

    /*
    GameObject* pGameObject = EngineFunctions::InstantiateGameObject<GameObject>();
    RenderMeshNode* pRenderNode = new RenderMeshNode();
    pRenderNode->setMesh(AssetManager<Mesh>::retrieve("sphere_mesh"));

    EngineFunctions::AssignRenderNode(pGameObject, pRenderNode);*/

    //nlohmann::json sceneJSON = Scene::JSONRepresentation::serialize(EngineFunctions::scene());
    //Loader::saveFile(std::filesystem::path("jsonTest/scene.json"), sceneJSON);


    m_windowIO.dropCallback().addCallbackFront([this](std::filesystem::path file) {

        std::filesystem::path extension = file.extension();
        if (extension != ".fbx") {
            return false;
        }

        //load mesh
        std::vector<MeshData*> vMeshes;
        if (!Loader::readMeshData(file, vMeshes)) {

            return true;
        }

        if (!ServiceLocator<Factory<GameObject>>::hasService()) {
            return true;
        }

        Factory<GameObject>& gameObjectFactory = *ServiceLocator<Factory<GameObject>>::retrieve();

        //GameObject* pGameObject;
        //gameObjectFactory.create("gameobject", )

        }
    );

    m_physics.setTargetUpdateRate(180);
    m_physics.setMaxUpdatesPerFrame(4);
}

