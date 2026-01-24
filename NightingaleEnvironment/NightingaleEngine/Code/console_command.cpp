#include "pch.h"
#include "console.h"

/*
string ConsoleCommand::nextArg(string& args)
{
    string arg = nextArgRaw(args);

    if (arg.size() >= 2) {
        bool bDefineSyntax = arg[0] == '$';
        if (bDefineSyntax) {
            assert(Console::pConsole != nullptr);
            map<string, string>& defines = Console::pConsole->m_executionState.defines;
            bool bDefineExists = defines.find(arg.substr(1)) != defines.end();
            if (bDefineExists) {
                arg = defines[arg.substr(1)];
            }
        }
    }

    return arg;
}

string ConsoleCommand::nextPath(string& args)
{
    return string();
}


void ExecutionState::verify()
{
    if (bCreateLog) {
        appendToLog(lastCommand, lastResult.message);
        if (!lastResult.bSuccess) {
            appendToError(lastCommand, lastResult.message);
        }
    }
}

void ExecutionState::appendToLog(string command, string message)
{
    log += " >" + command + "\n" + "    -->" + message + "\n\n";
}

void ExecutionState::appendToError(string command, string message)
{
    errorLog += " >" + command + "\n" + "    -->" + message + "\n\n";
}
*/