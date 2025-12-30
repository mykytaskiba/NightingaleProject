#pragma once
#include "execution_state.h"
#include "execution_result.h"

namespace afterparty {

    class Command;

    class AfterpartyEnvironment {

    public:
        //Must init environment to call commands
        void init();
        void tick();
        void shutdown();

        //Execute single command
        ExecutionResult execute(string const&);

        void registerSingleCommand(Command* command);

        static AfterpartyEnvironment* getInstance();

    private:

        static AfterpartyEnvironment* m_pInstance;//singleton

        ExecutionState m_executionState{};
        map<string, Command*> m_commandMap;


    };

}