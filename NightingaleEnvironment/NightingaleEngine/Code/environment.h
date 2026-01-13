#pragma once
#include "execution_state.h"
#include "execution_result.h"


    class CommandInterface;

    class ScriptingEnvironment {

    public:
        //Must init environment to call commands
        void init();
        void tick();
        void shutdown();

        //Execute single command
        ExecutionResult execute(string const&);

        void registerSingleCommand(CommandInterface* command);

        static ScriptingEnvironment* getInstance();

    private:

        static ScriptingEnvironment* m_pInstance;//singleton

        ExecutionState m_executionState{};
        map<string, CommandInterface*> m_commandMap;


    };
