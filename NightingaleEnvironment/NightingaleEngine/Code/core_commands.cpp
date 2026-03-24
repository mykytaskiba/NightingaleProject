#include "pch.h"
#include "core_commands.h"
#include "loader.h"
#include "console.h"
#include "scripting.h"
#include "argument.h"
#include "engine_functions.h"
#include "hotkey_processing.h"
/*

ExecutionResult VerifyCommand::execute(string args, ExecutionState& state)
{
    ExecutionResult result;
    result.bSuccess = true;

    if (state.lastResult.bSuccess) {
        result.message = state.lastCommand + " was successful";
        if (state.bCreateLog) {
            result.message = "success";
        }
    }
    else {
        result.message = state.lastCommand + " was not successful";
    }

    return result;
}

ExecutionResult DropStateCommand::execute(string args, ExecutionState& state)
{
    state = ExecutionState();
    
    ExecutionResult result;
    result.bSuccess = true;
    result.message = "previous command state dropped";
    return result;
}

ExecutionResult HelpCommand::execute(string args, ExecutionState& state)
{
}


ExecutionResult DefineCommand::execute(string args, ExecutionState& state)
{
    string defineAs = nextArgRaw(args);
    string defineVal = nextArgRaw(args);
    string rest = nextArgRaw(args);

    string expected = "define $(defineAs) (defineVal)";

    
    if (defineAs == "" || defineVal == "" || rest != "") {
        return ExecutionResult::FAIL("expected syntax: " + expected);
    }
    if (defineAs[0] != '$') {
        return ExecutionResult::FAIL("defineAs value must be preceeded by $ char: " + expected);
    }
    if (defineAs.size() == 1) {
        return ExecutionResult::FAIL("defineAs value cannot be nothing: " + expected);
    }

    defineAs = defineAs.substr(1);

    state.defines[defineAs] = defineVal;

    return ExecutionResult::SUCCESS("defined " + defineAs + " as " + defineVal);
}

ExecutionResult UndefineCommand::execute(string args, ExecutionState& state)
{
    string undefine = nextArgRaw(args);
    string rest = nextArgRaw(args);

    string expected = "undefine $(define)";


    if (undefine == "" || rest != "") {
        return ExecutionResult::FAIL("expected syntax: " + expected);
    }
    if (undefine[0] != '$') {
        return ExecutionResult::FAIL("defineAs value must be preceeded by $ char: " + expected);
    }
    if (undefine.size() == 1) {
        return ExecutionResult::FAIL("defineAs value cannot be nothing: " + expected);
    }

    undefine = undefine.substr(1);

    auto it = state.defines.find(undefine);
    if (it == state.defines.end()) {
        return ExecutionResult::FAIL("define not found $" + undefine);
    }
    state.defines.erase(it);

    return ExecutionResult::SUCCESS("undefined " + undefine);
}

ExecutionResult UndefineAllCommand::execute(string args, ExecutionState& state)
{
    state.defines.clear();
    return ExecutionResult::SUCCESS("undefined all");
}

ExecutionResult EnableLogCommand::execute(string args, ExecutionState& state)
{
    bool value;
    string expected = "@enablelog (true/false)";

    if (!nextBool(args, value)) {
        return ExecutionResult::FAIL("Expected syntax: " + expected);
    }
    if (nextArg(args) != "") {
        return ExecutionResult::FAIL("Expected syntax: " + expected);
    }

    string logDisabledMsg = "Logging disabled";
    if (state.bCreateLog && !value) {
        state.appendToLog(m_command + " false", logDisabledMsg);
    }
    state.bCreateLog = value;

    string msg = value ? "Logging enabled" : logDisabledMsg;
    return ExecutionResult::SUCCESS(msg);
}

ExecutionResult ClearConsoleLogCommand::execute(string args, ExecutionState& state)
{

    string expected = "@clearlog";
    if (nextArg(args) != "") {
        return ExecutionResult::FAIL("Expected syntax: " + expected);
    }

    state.log == "";
    return ExecutionResult::SUCCESS("Log was cleared");
}

ExecutionResult ShowConsoleLogCommand::execute(string args, ExecutionState& state)
{
    string expected = "@showlog";
    if (nextArg(args) != "") {
        return ExecutionResult::FAIL("Expected syntax: " + expected);
    }

    ExecutionResult result;
    result.bSuccess = true;
    result.message = state.log;
    return result;
}

ExecutionResult ShowErrorLogCommand::execute(string args, ExecutionState& state)
{
    string expected = "@showlog";
    if (nextArg(args) != "") {
        return ExecutionResult::FAIL("Expected syntax: " + expected);
    }

    ExecutionResult result;
    result.bSuccess = true;
    result.message = state.errorLog;
    return result;
}*/


void EchoCommand::execute_command(ArgumentList<AllText>& args, ExecutionState& state, ExecutionResult& result)
{
    result.bSuccess = true;

    AllText const& text = args.get<0>();
    result.message = (*text);
}

void ExecuteCommand::execute_command(ArgumentList<Line>& args, ExecutionState& state, ExecutionResult& result)
{
    std::string path = *args.get<0>();

    result.bSuccess = false;

    std::vector<std::string> command_contents;
    if (!Loader::readFile({ path }, command_contents)) {
        result.message = "failed reading file: " + path;
        return;
    }

    ScriptingEnvironment& scripting_env = *ScriptingEnvironment::getInstance();
    for (auto it = command_contents.begin(); it != command_contents.end(); ++it) {
        scripting_env.execute(*it);
    }

    result.bSuccess = true;
    result.message = "execution success";
    return;
}

void HelpCommand::execute_command(ArgumentList<Nothing>& args, ExecutionState& state, ExecutionResult& result)
{
    string commandList = "";

    assert(ScriptingEnvironment::getInstance() != nullptr);
    ScriptingEnvironment& scriptingEnv = *ScriptingEnvironment::getInstance();

    map<string, CommandInterface*> const& commandMap = scriptingEnv.m_commandMap;

    for (auto it = commandMap.cbegin(); it != commandMap.cend(); ++it) {
        string usage = (*it).second->getUsage();
        commandList += (*it).first + usage + "\n";
    }

    result.bSuccess = true;
    result.message = commandList;
    return;
}

void DefineCommand::execute_command(ArgumentList<Line, Line>& args, ExecutionState& state, ExecutionResult& result)
{
    string defineKey = *args.get<0>();
    string defineVal = *args.get<1>();

    state.defines[defineKey] = defineVal;

    result.bSuccess = true;
    result.message = defineKey + " was set to " + defineVal;
}

void SetWindowTitleCommand::execute_command(ArgumentList<AllText>& args, ExecutionState& state, ExecutionResult& result)
{
    string windowTitle = *args.get<0>();

    EngineFunctions::SetWindowTitle(windowTitle);

    result.bSuccess = true;
    result.message = "Window title set to " + windowTitle;
}

void QuitCommand::execute_command(ArgumentList<Nothing>& args, ExecutionState& state, ExecutionResult& result)
{
    EngineFunctions::close_app();

    result.bSuccess = true;
    result.message = "App quit";
}

void SetTargetFramerateCommand::execute_command(ArgumentList<uint>& args, ExecutionState& state, ExecutionResult& result)
{
    uint framerate = args.get<0>();
    EngineFunctions::SetTargetFramerate(framerate);

    result.bSuccess = true;
    result.message = "Target framerate set to " + std::to_string(framerate);
}

void HotkeyCommand::execute_command(ArgumentList<KeySequence, AllText>& args, ExecutionState& state, ExecutionResult& result)
{
    std::string executedCommand = *args.get<1>();
    Hotkey hotkey{
        args.get<0>().m_sequence ,
        [executedCommand]() {
            EngineFunctions::ExecuteCommand(executedCommand);
        }
    };
    EngineFunctions::hotkeyManager().addHotkey(hotkey);

    result.bSuccess = true;
    result.message = "Added a hotkey";
}

void ClearHotkeyCommand::execute_command(ArgumentList<KeySequence>& args, ExecutionState& state, ExecutionResult& result)
{
    EngineFunctions::hotkeyManager().clearHotkey(args.get<0>().m_sequence);

    result.bSuccess = true;
    result.message = "Removed a hotkey";
}
