#include "pch.h"
#include "console_command.h"
#include "console.h"

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

bool ConsoleCommand::nextBool(string& args, bool& value)
{

    string valueStr = nextArg(args);

    if (valueStr == "true") {
        value = true;
        return true;
    }
    if (valueStr == "false") {
        value = false;
        return true; //parsing was successful
    }
    return false;
}

string ConsoleCommand::nextArgRaw(string& args)
{
    string result;
    size_t pos = args.find(' ');
    if (pos == string::npos) {
        result = args;
        args = "";
        return result;
    }
    result = args.substr(0, pos);
    args = args.substr(pos + 1);
    return result;
}

bool ConsoleCommand::nextFloat(string& args, float& value)
{
    string valueStr = nextArg(args);
    try {
        value = std::stof(valueStr);
    }
    catch (std::invalid_argument const& e) {
        return false;
    }
    catch (std::out_of_range const& e) {
        return false;
    }
    return true;
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
