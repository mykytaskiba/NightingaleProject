#pragma once
#include "defines.h"
#include "execution_result.h"
#include "execution_state.h"
#include "argument.h"

//pure virtual core command interface
class CommandInterface {

public:
	virtual string const& getCommand() const = 0;
	virtual string getUsage() const = 0;
	virtual void execute(string args, ExecutionState& state, ExecutionResult& result) = 0;
};

//argument command
template<typename... Args>
class Command : public CommandInterface {
protected:
	string m_command{""};
	ArgumentList<Args...> m_arguments{};
public:

	Command(string const& commandStr) : m_command(commandStr) {}

	string const& getCommand() const override {
		return m_command;
	}

	string getUsage() const override {
		return m_arguments.get_usage();
	}

	virtual void execute(string args, ExecutionState& state, ExecutionResult& result) override {
		//m_arguments.parse(args);
		//verify result

		ParsingResult parsingState = m_arguments.parse(args);
		if (!parsingState.bSuccess) {
			result.bSuccess = false;
			result.message = parsingState.errorMessage;
			return;
		}
		execute_command(m_arguments, state, result);
	}

	virtual void execute_command(ArgumentList<Args...>& args, ExecutionState& state, ExecutionResult& result) = 0;
};
