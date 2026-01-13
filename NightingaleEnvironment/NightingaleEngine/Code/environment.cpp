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

    string command = ArgumentHelpers::getNextWithDefines(args);
    auto it = m_commandMap.find(command);
    bool bFoundCommand = it != m_commandMap.end();
    if (!bFoundCommand) {
        result.message = "Command not found";
        result.bValidCommand = false;
        return result;
    }

    m_executionState.lastCommand = commandIn;

    CommandInterface* const& pCommand = (*it).second;
    pCommand->execute(args, m_executionState, result);

    //m_executionState.verify();

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

