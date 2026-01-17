#include "pch.h"
#include "core_commands.h"
#include "engine_settings.h"
#include "engine.h"
#include "loader_commands.h"


void Engine::defaultSettings_CoreCommands() {
    //Core commands
    m_settings.console_commands.push_back(new EchoCommand());
    m_settings.console_commands.push_back(new ExecuteCommand());
    m_settings.console_commands.push_back(new HelpCommand());

    //Loader commands
    m_settings.console_commands.push_back(new ShaderLoadCommand());
    m_settings.console_commands.push_back(new MeshLoadCommand());
    m_settings.console_commands.push_back(new SkeletonLoadCommand());
    m_settings.console_commands.push_back(new AnimationLoadCommand());
    m_settings.console_commands.push_back(new LoaderScaleCommand());
}

/*
    //Console is ready when the commands are registered, so the singleton can be registered here
    pConsole = this;
    
    //All console commands must be registered here
    //Core commands
    registerSingleCommand(new EchoCommand());
    registerSingleCommand(new CPackCommand());
    registerSingleCommand(new VerifyCommand());
    registerSingleCommand(new DropStateCommand());
    registerSingleCommand(new DefineCommand());
    registerSingleCommand(new UndefineCommand());
    registerSingleCommand(new UndefineAllCommand());
    registerSingleCommand(new EnableLogCommand());
    registerSingleCommand(new ClearConsoleLogCommand());
    registerSingleCommand(new ShowConsoleLogCommand());
    registerSingleCommand(new ShowErrorLogCommand());


    //logic commands
    registerSingleCommand(new CreateGameObjectCommand());
    aliasCommand("create", new CreateGameObjectCommand());
    registerSingleCommand(new AttachRenderMeshCommand());
    registerSingleCommand(new AttachAnimatedMeshCommand());
    registerSingleCommand(new SetPositionCommand());
    registerSingleCommand(new SetScaleCommand());
    aliasCommand("setpos", new SetPositionCommand());
    registerSingleCommand(new SetColorCommand());*/