#include "pch.h"
#include "environment.h"
#include "command.h"
#include "argument.h"

using namespace afterparty;

AfterpartyEnvironment* AfterpartyEnvironment::m_pInstance = nullptr;

void AfterpartyEnvironment::init()
{
    m_pInstance = this;
}

void AfterpartyEnvironment::tick()
{
}

void AfterpartyEnvironment::shutdown()
{
}

ExecutionResult AfterpartyEnvironment::execute(string const& commandIn)
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

    string command = Argument::extractNext(args);
    auto it = m_commandMap.find(command);
    bool bFoundCommand = it != m_commandMap.end();
    if (!bFoundCommand) {
        result.message = "Command not found";
        result.bValidCommand = false;
        return result;
    }

    m_executionState.lastCommand = commandIn;

    Command* const& pCommand = (*it).second;
    pCommand->execute(args, m_executionState, result);

    //m_executionState.verify();

    return result;
}

void AfterpartyEnvironment::registerSingleCommand(Command* command)
{
    assert(command != nullptr);
    string const& commandStr = command->getCommand();
    assert(m_commandMap.find(commandStr) == m_commandMap.end());
    
    m_commandMap[commandStr] = command;
}

AfterpartyEnvironment* AfterpartyEnvironment::getInstance()
{
    return m_pInstance;
}

