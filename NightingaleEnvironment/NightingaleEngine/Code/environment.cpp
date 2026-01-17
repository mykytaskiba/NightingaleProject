#include "pch.h"
#include "environment.h"
#include "command.h"
#include "argument.h"
#include "argument_helpers.h"


ScriptingEnvironment* ScriptingEnvironment::m_pInstance = nullptr;

void ScriptingEnvironment::init()
{
    m_pInstance = this;
}

void ScriptingEnvironment::tick()
{
}

void ScriptingEnvironment::shutdown()
{
}

ExecutionResult ScriptingEnvironment::execute(string const& commandIn)
{
    ExecutionResult result;

    string args(commandIn);

    if (args.size() == 0) {

        result.message = "No command entered";
        result.bValidCommand = false;
        return result;
    }
    
    if (args.size() >= 2) {
        if (commandIn.at(0) == '/' && commandIn.at(1) == '/') {
            result.bValidCommand = false;
            return result;
        }
    }

    preprocessCommand(args);
    string command = ArgumentHelpers::getNextWithDefines(args);
    auto it = m_commandMap.find(command);
    bool bFoundCommand = it != m_commandMap.end();
    if (!bFoundCommand) {
        result.message = "command not found";
        result.bValidCommand = false;
        return result;
    }

    m_executionState.lastCommand = commandIn;

    CommandInterface* const& pCommand = (*it).second;
    pCommand->execute(args, m_executionState, result);

    return result;
}

void ScriptingEnvironment::registerSingleCommand(CommandInterface* command)
{
    assert(command != nullptr);
    string const& commandStr = command->getCommand();
    assert(m_commandMap.find(commandStr) == m_commandMap.end());
    
    m_commandMap[commandStr] = command;
}

ScriptingEnvironment* ScriptingEnvironment::getInstance()
{
    return m_pInstance;
}

void ScriptingEnvironment::preprocessCommand(string& command) const
{
    if (defineSyntax(command)) return;

    string processedCommand;

    string commandStr = ArgumentHelpers::getNextRaw(command);
    auto it = m_commandMap.find(commandStr);
    bool bFoundCommand = it != m_commandMap.end();

    if (!bFoundCommand) {
        commandStr = autoFillCommand(commandStr);
    }

    processedCommand = commandStr + " " + command;
    command = processedCommand;
}


string ScriptingEnvironment::autoFillCommand(string const& inCommand) const
{
    string filledCommand = ".execute data/" + inCommand + ".ngs";
    return filledCommand;
}

bool ScriptingEnvironment::defineSyntax(string& inCommand) const
{
    size_t equalSign = inCommand.find('=');
    if (equalSign == string::npos) {
        return false;
    }

    string defineKey = inCommand.substr(0, equalSign);
    string defineVal = inCommand.substr(equalSign+1);
    trimWhitespace(defineKey);
    trimWhitespace(defineVal);

    inCommand = ".define " + defineKey + " " + defineVal;

}

void ScriptingEnvironment::trimWhitespace(string& inStr) const
{
    inStr = inStr.substr(inStr.find_first_not_of(' '));
    inStr = inStr.substr(0, inStr.find_last_not_of(' ') + 1);
}

