#include "pch.h"

#include "engine.h"
#include "logger.h"
#include "termination.h"
#include "time.h"

#include "engine_internals.h"
#include "console.h"
#include "engine_functions.h"


void Engine::run()
{
    wakeUp();

    init();

    update();

    shutdown();



}

void Engine::wakeUp()
{
    //Logger::SetLoggingStatus(LoggerStatus::LOG_FULL);
    //Logger::Log("Nightingale Engine has woken up");
}

void Engine::init()
{
    EngineInternals::init(*this);

    m_window.init();

    m_frameController.setTargetFrameRate(60);

    m_debugUI.init(m_window);

    m_input.init();

    m_scene.init();

    EngineFunctions::InstantiateGameObject<Console>();
    m_scene.tick(); //tick once so the console is created
    
    ExecutionResult result = Console::ExecuteFromString("cpack data/load.cpack");
    assert(result.bSuccess);

    m_renderer.init();

}

void Engine::update()
{

    while (!Termination::NeedTermination())
    {
        m_frameController.frameStart();
        

        m_debugUI.newFrame();
        m_input.captureInputState();

        m_scene.tick();


        m_renderer.render();
        m_debugUI.endFrame();

        m_window.update();

        m_frameController.frameEnd();
        Time::s_delta = m_frameController.getDeltaTime();
        

    }
}

void Engine::shutdown()
{
    m_scene.shutdown();
    m_input.shutdown();
    m_debugUI.shutdown();
    m_renderer.shutdown();
    m_window.shutdown();
}
