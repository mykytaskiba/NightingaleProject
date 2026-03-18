#include "pch.h"
#include "core_commands.h"
#include "engine_settings.h"
#include "engine.h"
#include "loader_commands.h"
#include "gameobject_commands.h"
#include "debug_panel_commands.h"
#include "physics_control_panel.h"
#include "physics_commands.h"
#include "lag_simulator.h"
#include "scene_browser.h"


void Engine::defaultSettings_CoreCommands() {
    //Core commands
    m_settings.console_commands.push_back(new EchoCommand());
    m_settings.console_commands.push_back(new ExecuteCommand());
    m_settings.console_commands.push_back(new HelpCommand());
    m_settings.console_commands.push_back(new DefineCommand());
    m_settings.console_commands.push_back(new SetWindowTitleCommand());
    m_settings.console_commands.push_back(new QuitCommand());
    m_settings.console_commands.push_back(new SetTargetFramerateCommand());
    m_settings.console_commands.push_back(new HotkeyCommand());
    m_settings.console_commands.push_back(new ClearHotkeyCommand());

    //Loader commands
    m_settings.console_commands.push_back(new ShaderLoadCommand());
    m_settings.console_commands.push_back(new MeshLoadCommand());
    m_settings.console_commands.push_back(new SkeletonLoadCommand());
    m_settings.console_commands.push_back(new AnimationLoadCommand());

    //logic commands
    m_settings.console_commands.push_back(new CreateGameObjectCommand());
    m_settings.console_commands.push_back(new AttachRenderMeshCommand());
    m_settings.console_commands.push_back(new AttachAnimatedMeshCommand());
    m_settings.console_commands.push_back(new SetPositionCommand());
    m_settings.console_commands.push_back(new SetScaleCommand());
    m_settings.console_commands.push_back(new SetColorCommand());

    //debug panels
    m_settings.console_commands.push_back(new PanelCommand<EngineMenuBar>("engine_menu"));
    m_settings.console_commands.push_back(new PanelCommand<RenderDebugPanel>("render_panel"));
    m_settings.console_commands.push_back(new PanelCommand<ShaderResourceBrowser>("shader_browser"));
    m_settings.console_commands.push_back(new PanelCommand<MeshBrowser>("mesh_browser"));
    m_settings.console_commands.push_back(new PanelCommand<PhysicsControlPanel>("physics_control"));
    m_settings.console_commands.push_back(new PanelCommand<LagSimulatorPanel>("lag_simulator"));
    m_settings.console_commands.push_back(new PanelCommand<Console>("console"));
    m_settings.console_commands.push_back(new PanelCommand<SceneHierarchy>("scene_hierarchy"));
    m_settings.console_commands.push_back(new PanelCommand<ImGUIDemoPanel>("imgui_demo"));
    m_settings.console_commands.push_back(new PanelCommand<Inspector>("inspector"));

    //physics commands
    m_settings.console_commands.push_back(new PhysicsActiveCommand());
}


/*
    //Console is ready when the commands are registered, so the singleton can be registered here
    pConsole = this;
    
    //All console commands must be registered here
    //Core commands
    registerSingleCommand(new VerifyCommand());
    registerSingleCommand(new DropStateCommand());
    registerSingleCommand(new DefineCommand());
    registerSingleCommand(new UndefineCommand());
    registerSingleCommand(new UndefineAllCommand());
    registerSingleCommand(new EnableLogCommand());
    registerSingleCommand(new ClearConsoleLogCommand());
    registerSingleCommand(new ShowConsoleLogCommand());
    registerSingleCommand(new ShowErrorLogCommand());

*/