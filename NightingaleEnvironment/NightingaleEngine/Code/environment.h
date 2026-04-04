#pragma once
#include "execution_state.h"
#include "execution_result.h"


    class CommandInterface;

    class ScriptingEnvironment {
        friend class HelpCommand; //to help list all commands
    public:
        //Must init environment to call commands
        void init();

        //Execute single command
        ExecutionResult execute(std::string const&);

        void registerSingleCommand(CommandInterface* command);

        static ScriptingEnvironment* getInstance();

        void preprocessCommand(std::string&) const;
        //autofill an execution command, i.e. "sample_script" -> "execute_command path/sample_script.ext"
        std::string autoFillCommand(std::string const&) const;

        bool defineSyntax(std::string&) const;

        void trimWhitespace(std::string&) const;

        ExecutionState const& getExecutionState() const { return m_executionState; }
        ExecutionState& getExecutionState() { return m_executionState; }

    private:

        static ScriptingEnvironment* m_pInstance;//singleton

        ExecutionState m_executionState{};
        std::map<std::string, CommandInterface*> m_commandMap;


    };
