#pragma once
#include "console_command.h"


class EchoCommand : public ConsoleCommand {
public:
    EchoCommand() : ConsoleCommand("echo") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

//a cpack is a console pack
class CPackCommand : public ConsoleCommand {
public:
    CPackCommand() : ConsoleCommand("cpack") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

//verify last command was successful
class VerifyCommand : public ConsoleCommand {
public:
    VerifyCommand() : ConsoleCommand("@verify") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

//Drop state from the previous command entirely
class DropStateCommand : public ConsoleCommand {
public:
    DropStateCommand() : ConsoleCommand("dropstate") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

class HelpCommand : public ConsoleCommand {
public:
    HelpCommand() : ConsoleCommand("help") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

class DefineCommand : public ConsoleCommand {
public:
    DefineCommand() : ConsoleCommand("@define") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

class UndefineCommand : public ConsoleCommand {
public:
    UndefineCommand() : ConsoleCommand("@undefine") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

class UndefineAllCommand : public ConsoleCommand {
public:
    UndefineAllCommand() : ConsoleCommand("@undefine_all") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

class EnableLogCommand : public ConsoleCommand {
public:
    EnableLogCommand() : ConsoleCommand("@enablelog") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);

};

class ClearConsoleLogCommand : public ConsoleCommand {
public:
    ClearConsoleLogCommand() : ConsoleCommand("@clearlog") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

class ShowConsoleLogCommand : public ConsoleCommand {
public:
    ShowConsoleLogCommand() : ConsoleCommand("@showlog") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

class ShowErrorLogCommand : public ConsoleCommand {
public:
    ShowErrorLogCommand() : ConsoleCommand("@showerror") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};
