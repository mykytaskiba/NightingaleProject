#pragma once
#include "game_object.h"
#include "defines.h"
#include "console_command.h"

class ConsoleCommand;

class Console {
    friend class HelpCommand;
    friend class ConsoleCommand;
    friend class Engine;
public:
    //static context execute command
    static ExecutionResult ExecuteFromString(string command);

protected:


    static Console* pConsole;

    bool m_consoleShown{ false }; //toggle by Tilda key
    uint m_consoleHeight = 100; //100 pixels?

    ExecutionState m_executionState;

    string m_consoleBuffer;

    string m_consoleMessage;

    map<string, ConsoleCommand*> m_commandMap;


    void registerAllCommands();
    void registerSingleCommand(ConsoleCommand* command);

    void aliasCommand(string alias, ConsoleCommand* command);


    ExecutionResult executeCommand(string args);

    vector<string> m_executedCommands;
    size_t m_executedCommandBrowser;

    virtual void init();
    virtual void tick();
    virtual void shutdown();
};