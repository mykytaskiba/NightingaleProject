#include "pch.h"
#include "core_commands.h"
#include "engine_settings.h"
#include "engine.h"


void Engine::defaultSettings_CoreCommands() {
    m_settings.console_commands.push_back(new EchoCommand());
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
    registerSingleCommand(new HelpCommand());
    registerSingleCommand(new DefineCommand());
    registerSingleCommand(new UndefineCommand());
    registerSingleCommand(new UndefineAllCommand());
    registerSingleCommand(new EnableLogCommand());
    registerSingleCommand(new ClearConsoleLogCommand());
    registerSingleCommand(new ShowConsoleLogCommand());
    registerSingleCommand(new ShowErrorLogCommand());

    //Loader commands
    registerSingleCommand(new ShaderLoadCommand());
    registerSingleCommand(new MeshLoadCommand());
    registerSingleCommand(new SkeletonLoadCommand());
    registerSingleCommand(new AnimationLoadCommand());
    registerSingleCommand(new LoaderScaleCommand());

    //logic commands
    registerSingleCommand(new CreateGameObjectCommand());
    aliasCommand("create", new CreateGameObjectCommand());
    registerSingleCommand(new AttachRenderMeshCommand());
    registerSingleCommand(new AttachAnimatedMeshCommand());
    registerSingleCommand(new SetPositionCommand());
    registerSingleCommand(new SetScaleCommand());
    aliasCommand("setpos", new SetPositionCommand());
    registerSingleCommand(new SetColorCommand());*/