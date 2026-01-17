#pragma once
#include "command.h"

class EchoCommand : public Command<AllText> {
public:
    EchoCommand() : Command<AllText>("echo") {}
    virtual void execute_command(ArgumentList<AllText>& args, ExecutionState& state, ExecutionResult& result) override;
};

class ExecuteCommand : public Command<Line> {
public:
    ExecuteCommand() : Command(".execute") {}
    virtual void execute_command(ArgumentList<Line>& args, ExecutionState& state, ExecutionResult& result) override;
};


class DefineCommand : public Command<Line,Line> {
public:
    DefineCommand() : Command(".define") {}
    virtual void execute_command(ArgumentList<Line, Line>& args, ExecutionState& state, ExecutionResult& result) override;
};

class HelpCommand : public Command<Nothing> {
public:
    HelpCommand() : Command("help") {}
    virtual void execute_command(ArgumentList<Nothing>& args, ExecutionState& state, ExecutionResult& result) override;
};
/*

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