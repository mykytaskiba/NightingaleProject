#include "pch.h"
#include "console.h"
#include "input.h"
#include "engine_functions.h"
#include "core_commands.h"
#include "loader_commands.h"
#include "gameobject_commands.h"

/*
Console* Console::pConsole = nullptr;

ExecutionResult Console::ExecuteFromString(string args)
{
    assert(pConsole != nullptr);
    return pConsole->executeCommand(args);
}
void Console::registerAllCommands()
{
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
    registerSingleCommand(new SetColorCommand());
}*/
/*
void Console::registerSingleCommand(ConsoleCommand* command)
{
    assert(command != nullptr);
    m_commandMap[command->getCommand()] = command;
}*/
/*
void Console::aliasCommand(string alias, ConsoleCommand* command)
{

    assert(command != nullptr);
    m_commandMap[alias] = command;

}*/
/*
ExecutionResult Console::executeCommand(string args)
{
    if (args.size() == 0) {
        return ExecutionResult::FAIL("no command entered");
    }
    if (args.size() >= 2) {
        if (args.at(0) == '/' && args.at(1) == '/') {
            ExecutionResult result;
            result.bSuccess = false;
            result.message = "this is a comment";
            return result;
        }
    }

    string command_full(args);
    m_executionState.lastCommand = command_full;

    string command = ConsoleCommand::nextArg(args);
    auto it = m_commandMap.find(command);
    bool bValidCommand = it != m_commandMap.end();

    ExecutionResult result;
    if (!bValidCommand) {
        result.message = "Command not valid: " + command;
        result.bSuccess = false;

    }
    else {
        result = (*it).second->execute(args, m_executionState);
    }

    m_executionState.lastResult = result;
    m_executionState.verify();

    return result;
}*/


void Console::init()
{
}

void Console::tick()
{
    bool bShowConsolePressed = Input::KeyClicked(Key::Tilda);
    if (bShowConsolePressed) {
        m_consoleShown = !m_consoleShown;
        m_consoleBuffer.clear();
        ImGui::SetNextWindowFocus();
    }

    if (m_consoleShown) {
        int screenWidth = EngineFunctions::ScreenWidth();
        
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(screenWidth, m_consoleHeight));

        bool hasPreviousExecutedCommands = (m_executedCommands.size() != 0);
        

        if (Input::KeyClicked(Key::UpArrow) && hasPreviousExecutedCommands) {
            uint commandIndex = m_executedCommands.size() - m_executedCommandBrowser - 1;
                m_consoleBuffer = m_executedCommands[commandIndex];

                if (m_executedCommandBrowser + 1u < m_executedCommands.size()) {
                    m_executedCommandBrowser++;
                }
        }
        if (Input::KeyClicked(Key::DownArrow) && hasPreviousExecutedCommands) {
            if (m_executedCommandBrowser > 0) {
                m_executedCommandBrowser--;
            }
            
            uint commandIndex = m_executedCommands.size() - m_executedCommandBrowser - 1;
            m_consoleBuffer = m_executedCommands[commandIndex];
        }

        ImGui::Begin("Console", NULL, 
            ImGuiWindowFlags_NoCollapse | 
            ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoTitleBar);

        if (bShowConsolePressed) {
            ImGui::SetKeyboardFocusHere();
        }
        if (ImGui::InputText("<--", &m_consoleBuffer)) {
            m_executedCommandBrowser = 0;
        }
        ImGui::Text(m_consoleMessage.c_str());
        ImGui::End();

        //both keys are down and one of them was just clicked
        bool commandEntered =
            (Input::KeyDown(Key::Shift) && Input::KeyClicked(Key::Enter))
            ||
            (Input::KeyClicked(Key::Shift) && Input::KeyDown(Key::Enter));
            

        if (commandEntered) {
            string fullCommand = string(m_consoleBuffer);
            assert(ScriptingEnvironment::getInstance() != nullptr);
            ScriptingEnvironment& scriptingEnv = *ScriptingEnvironment::getInstance();
            ExecutionResult executionResult = scriptingEnv.execute(fullCommand);
            if (executionResult.bSuccess) {
                m_consoleBuffer.clear();
                m_executedCommands.push_back(fullCommand);
                m_executedCommandBrowser = 0;
            }
            m_consoleMessage = executionResult.message;
        }
    }
}

void Console::shutdown()
{
}
