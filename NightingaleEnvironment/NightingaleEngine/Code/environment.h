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
        ExecutionResult execute(string const&);

        void registerSingleCommand(CommandInterface* command);

        static ScriptingEnvironment* getInstance();

        void preprocessCommand(string&) const;
        //autofill an execution command, i.e. "sample_script" -> "execute_command path/sample_script.ext"
        string autoFillCommand(string const&) const;

        bool defineSyntax(string&) const;

        void trimWhitespace(string&) const;

    private:

        static ScriptingEnvironment* m_pInstance;//singleton

        ExecutionState m_executionState{};
        map<string, CommandInterface*> m_commandMap;


    };
