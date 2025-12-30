#pragma once
#include "defines.h"
#include "execution_result.h"
#include "execution_state.h"
#include "argument.h"

namespace afterparty {

    class ExecutionState;

    //a single command
    class Command {

    protected:
        string m_command;
        

    private:

    public:
        Command(string command);

        string const& getCommand() const { return m_command; }
        //returns a message with the command result
        virtual void execute(string args, ExecutionState& state, ExecutionResult& result);

    protected:
        bool verifyArgumentResult(ArgumentExtractionResult const& argumentExtraction, ExecutionResult& commandResult);
    };

}