#pragma once
#include "defines.h"
#include "renderer.h"
#include "frame_controller.h"
#include "debug_ui.h"
#include "input.h"
#include "window.h"
#include "screen.h"
#include "scene.h"
#include "camera.h"
#include "asset_manager.h"
#include "console.h"
#include "engine_settings.h"
#include "afterparty.h"

#define DEFAULT_FPS_COUNT 30 


class Engine {
    friend class EngineInternals;
    friend class EngineFunctions;
public:

    EngineSettings& settings();
    void run();

    Engine();

private:

    //Core loop, init -> update -> shutdown
    void init();
    void update();
    void shutdown();

    void registerConsoleCommands();
    void loadCommands();
    
    EngineSettings m_settings;
    void setDefaultSettings();
    void defaultSettings_CoreCommands();

    Window m_window;
    Renderer m_renderer;
    Input m_input;
    DebugUI m_debugUI;
    Console m_console;
    AfterpartyEnvironment m_scriptEnvironment;
    
    Scene m_scene;
    Camera m_camera; //for now assume a single camera

    static Engine* m_pEngine;

    FrameController m_frameController = FrameController(DEFAULT_FPS_COUNT);
};