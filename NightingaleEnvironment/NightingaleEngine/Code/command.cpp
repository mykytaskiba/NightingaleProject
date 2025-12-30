#include "pch.h"
#include "command.h"

using namespace afterparty;



Command::Command(string command) : m_command(command)
{
}

void Command::execute(string args, ExecutionState& state, ExecutionResult& result)
{
    assert(false);
}

bool Command::verifyArgumentResult(ArgumentExtractionResult const& argumentExtraction, ExecutionResult& commandResult)
{
    commandResult.bSuccess = argumentExtraction.bSuccess;

    commandResult.message = argumentExtraction.errorMessage;
    commandResult.message += "\n" + m_command + argumentExtraction.sampleUsage;
    return commandResult.bSuccess;
}

