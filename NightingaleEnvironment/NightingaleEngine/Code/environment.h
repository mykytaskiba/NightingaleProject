#pragma once
#include "execution_state.h"

namespace afterparty {



    class AfterpartyEnvironment {

    public:
        //Must init environment to call commands
        void init();

        //Execute single command
        void execute(string const&);

    private:
        ExecutionState m_executionState{};


    };

}