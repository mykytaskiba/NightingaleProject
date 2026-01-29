#pragma once
#include "defines.h"
#include "renderer.h"
#include "frame_controller.h"
#include "debug_ui.h"
#include "input.h"
#include "window.h"
#include "screen.h"
#include "scene.h"
#include "asset_manager.h"
#include "console.h"
#include "engine_settings.h"
#include "scripting.h"
#include "priority_function.h"
#include "ngphys.h"
#include "ngrender.h"



class Engine {
    friend class EngineInternals;
    friend class EngineFunctions;
    friend class EngineSystemBinding;
public:

    EngineSettings& settings();
    void run();

    Engine();

private:

    //Core loop, init -> update -> shutdown
    void register_systems();
    void init();
    void update();
    void shutdown();

    void registerConsoleCommands();
    void loadCommands();
    
    EngineSettings m_settings;
    void setDefaultSettings();
    void defaultSettings_CoreCommands();

    set<PriorityFunction> m_initFunctions;
    set<PriorityFunction> m_updateFunctions;
    set<PriorityFunction> m_shutdownFunctions;

    void executeFunctionsInSet(set<PriorityFunction>& set);
    void register_init(uint priority, FEngineProcedure function);
    void register_update(uint priority, FEngineProcedure function);
    void register_shutdown(uint priority, FEngineProcedure function);

    Window m_window;
    Renderer m_renderer;
    Input m_input;
    DebugUI m_debugUI;
    Console m_console;
    ScriptingEnvironment m_scriptEnvironment;
    Physics m_physics;
    FrameController m_frameController;
    
    Scene m_scene;
    Camera m_camera; //for now assume a single camera

    static Engine* m_pEngine;

    void CS550TempTestFuncInit();

};