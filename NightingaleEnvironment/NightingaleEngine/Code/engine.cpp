#include "pch.h"

#include "engine.h"
#include "logger.h"
#include "termination.h"

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
    register_init(74, [this] {m_window.init(); });
    register_init(75, [this] {m_debugUI.init(m_window); });
    register_init(80, [this] {m_input.init(); });
    register_init(90, [this] {m_scene.init(); });
    register_init(95, [this] {  loadCommands(); });
    register_init(100, [this] {m_renderer.init(); });
    register_init(110, [this] {CS550TempTestFuncInit(); });
    
    register_update(60, [this] {m_frameController.frameStart(); });
    register_update(70, [this] {m_debugUI.newFrame(); });
    register_update(80, [this] {m_input.captureInputState(); });
    register_update(90, [this] {m_scene.tick(); });
    register_update(100, [this] {m_console.tick(); });
    register_update(190, [this] {m_physics.update(m_frameController.getDeltaTime()); });
    register_update(200, [this] {m_renderer.render(); });
    register_update(230, [this] {m_debugUI.endFrame(); });
    register_update(240, [this] {m_window.update(); });
    register_update(250, [this] {m_frameController.frameEnd(); });

    register_shutdown(100, [this] {m_debugUI.shutdown(); });

}

void Engine::init()
{
    EngineInternals::init(*this);
    
    //Currently scripting environment is handled differently than the rest of system
    m_scriptEnvironment.init();
    registerConsoleCommands();

    executeFunctionsInSet(m_initFunctions);
}

void Engine::update()
{

    while (!Termination::NeedTermination())
    {
        executeFunctionsInSet(m_updateFunctions);
    }

    

}

void Engine::shutdown()
{
    executeFunctionsInSet(m_shutdownFunctions);
}

void Engine::registerConsoleCommands()
{
    for (CommandInterface*& command : m_settings.console_commands) {
        m_scriptEnvironment.registerSingleCommand(command);
    }
}

void Engine::loadCommands()
{
    for (string const command : m_settings.load_commands) {
        ExecutionResult result = m_scriptEnvironment.execute(command);
        assert(result.bSuccess);
    }
}

void Engine::executeFunctionsInSet(set<PriorityFunction>& set)
{
    for (auto it{ set.begin() }; it != set.end(); ++it) {
        (*it)();
    }
}

void Engine::register_init(uint priority, FEngineProcedure function)
{
    PriorityFunction func(priority, function);
    m_initFunctions.insert(func);
}

void Engine::register_update(uint priority, FEngineProcedure function)
{
    PriorityFunction func(priority, function);
    m_updateFunctions.insert(func);
}

void Engine::register_shutdown(uint priority, FEngineProcedure function)
{
    PriorityFunction func(priority, function);
    m_shutdownFunctions.insert(func);
}


void Engine::setDefaultSettings()
{
    m_settings.load_commands.push_back(".execute data/core/load.ngs");

    defaultSettings_CoreCommands();
}

#include "render_mesh.h"
#include "physics_debug_render_pass.h"
void Engine::CS550TempTestFuncInit()
{
    /*
    GameObject* pGameObject = EngineFunctions::InstantiateGameObject<GameObject>();
    RenderMeshNode* pRenderNode = new RenderMeshNode();
    pRenderNode->setMesh(AssetManager<Mesh>::retrieve("sphere_mesh"));

    EngineFunctions::AssignRenderNode(pGameObject, pRenderNode);
    */
    int ySize = 2;
    int xSize = 2;
    int zSize = 2;

    float offset = 3.0f;
    float cubeSize = 1.0f;

    float velocityFromCenter = 10.0f;
    Vector3 center = { 0,0,0 };

    for (int x = -xSize; x <= xSize; x++) {
        for (int z = -zSize; z <= zSize; z++) {
            for (int y = -ySize; y <= ySize; y++) {
                Vector3 position{ (float)x,(float)y,(float)z };
                position *= offset * cubeSize;
                    
                PhysicsBody* pBody = new PhysicsBody();
                pBody->localBox = AxisAlignedBox({ 0,0,0 }, { cubeSize,cubeSize,cubeSize });
                pBody->position = position;
                pBody->velocity = (position - center).normalized() * velocityFromCenter;
                m_physics.addBody(pBody);
                pBody->bUseGravity = true;

                pBody->update(0.0f);

                GameObject* pGameObject = EngineFunctions::InstantiateGameObject<GameObject>();
                RenderMeshNode* pRenderNode = new RenderMeshNode();
                pRenderNode->setMesh(AssetManager<Mesh>::retrieve("sphere_mesh"));

                EngineFunctions::AssignRenderNode(pGameObject, pRenderNode);
                pGameObject->m_pPhysicsBody = pBody;

            }
        }
    }

    //TO DO: This definitely creates a memory leak
    PhysicsDebugRenderPass* pPhysicsPass = new PhysicsDebugRenderPass(m_physics);
    m_renderer.registerRenderPass(pPhysicsPass);


    PhysicsBody* pFloor = new PhysicsBody();
    pFloor->localBox = AxisAlignedBox({ 0.0f,0.0f,0.0f }, { 150.0f,0.25f,150.0f });
    pFloor->position = Vector3(0, -10.0f, 0);
    pFloor->update(0.0f);
    m_physics.addBody(pFloor);

    m_physics.setTargetUpdateRate(180);
    m_physics.setMaxUpdatesPerFrame(4);
}