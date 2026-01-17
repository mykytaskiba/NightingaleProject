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
    init();

    update();

    shutdown();
}

Engine::Engine()
{
    setDefaultSettings();
}


void Engine::init()
{
    EngineInternals::init(*this);
    
    m_scriptEnvironment.init();
    registerConsoleCommands();


    m_window.init();
    m_window.setTitle(m_settings.window_title);

    m_frameController.setTargetFrameRate(60);

    m_debugUI.init(m_window);

    m_input.init();

    m_scene.init();

    m_console.init();
    
    loadCommands();

    m_renderer.init();


}

void Engine::update()
{

    while (!Termination::NeedTermination())
    {
        m_frameController.frameStart();
        

        m_debugUI.newFrame();
        m_input.captureInputState();
        
        m_scriptEnvironment.tick();
        m_scene.tick(); 
        m_console.tick();


        m_renderer.render();
        m_debugUI.endFrame();

        m_window.update();

        m_frameController.frameEnd();
        

    }
}

void Engine::shutdown()
{
    m_scene.shutdown();
    m_console.shutdown();
    m_input.shutdown();
    m_debugUI.shutdown();
    m_renderer.shutdown();
    m_window.shutdown();
    m_scriptEnvironment.shutdown();
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

void Engine::setDefaultSettings()
{
    m_settings.window_title = "Nightingale Engine";

    m_settings.load_commands.push_back(".execute data/core/load.ngs");

    defaultSettings_CoreCommands();
}
