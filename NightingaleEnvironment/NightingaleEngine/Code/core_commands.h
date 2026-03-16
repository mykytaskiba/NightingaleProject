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

class SetWindowTitleCommand : public Command<AllText> {
public:
    SetWindowTitleCommand() : Command("set_window_title") {}
    virtual void execute_command(ArgumentList<AllText>& args, ExecutionState& state, ExecutionResult& result) override;
};

class QuitCommand : public Command<Nothing> {
public:
    QuitCommand() : Command("quit") {}
    virtual void execute_command(ArgumentList<Nothing>& args, ExecutionState& state, ExecutionResult& result) override;
};

class SetTargetFramerateCommand : public Command<uint> {
public:
    SetTargetFramerateCommand() : Command("set_target_framerate") {}
    virtual void execute_command(ArgumentList<uint>& args, ExecutionState& state, ExecutionResult& result) override;
};

class HotkeyCommand : public Command<KeySequence, AllText> {
public:
    HotkeyCommand() : Command("hotkey") {}
    virtual void execute_command(ArgumentList<KeySequence, AllText>& args, ExecutionState& state, ExecutionResult& result) override;

};

class ClearHotkeyCommand : public Command<KeySequence> {
public:
    ClearHotkeyCommand() : Command("hotkey_clear") {}
    virtual void execute_command(ArgumentList<KeySequence>& args, ExecutionState& state, ExecutionResult& result) override;
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