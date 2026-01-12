#pragma once
#include "defines.h"
#include "execution_result.h"
#include "execution_state.h"
#include "argument.h"

namespace afterparty {

    class ExecutionState;
    /*
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
    };*/
    
    //pure virtual core command interface
    class CommandInterface {

    public:
        virtual string const& getCommand() const = 0;
        virtual void execute(string args, ExecutionState& state, ExecutionResult& result) = 0;
    };

    //argument command
    template<char const* CommandStr, typename... Args>
    class Command : public CommandInterface {
    private:
        string m_command{ CommandStr };
        ArgumentList<Args> m_arguments;
    public:
        string const& getCommand() override {
            return m_command;
        }

        void execute(string args, ExecutionState& state, ExecutionResult& result) override {
            //m_arguments.parse(args);
            //verify result
        }
    protected:
        virtual void execute_command(ArgumentList<Args> args, ExecutionState& state, ExecutionResult& result) = 0;
    };
}