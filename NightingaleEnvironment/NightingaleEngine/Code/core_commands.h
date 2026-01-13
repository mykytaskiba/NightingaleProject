#pragma once
#include "command.h"

class EchoCommand : public Command<string> {
public:
    EchoCommand() : Command<string>("echo") {}
    virtual void execute_command(ArgumentList<string>& args, ExecutionState& state, ExecutionResult& result) override;
};

/*
class CPackCommand : public ConsoleCommand {
public:
    CPackCommand() : ConsoleCommand("cpack") {}
    virtual ExecutionResult execute(string args, ExecutionState& state);
};

/*
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

*/