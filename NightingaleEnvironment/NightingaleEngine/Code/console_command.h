#pragma once
#include "defines.h"

class GameObject;
class RenderNode;

/*
struct ExecutionResult {
    string message{ "No command executed" };
    bool bSuccess{ false };

    static ExecutionResult FAIL(string const& message) {
        ExecutionResult result;
        result.bSuccess = false;
        result.message = message;
        return result;
    }

    static ExecutionResult SUCCESS(string const& message) {
        ExecutionResult result;
        result.bSuccess = true;
        result.message = message;
        return result;
    }
};


struct ExecutionState {
    string lastCommand{"No command"};
    ExecutionResult lastResult{};

    GameObject* pGameObject{ nullptr };
    RenderNode* pRenderNode{ nullptr };

    map<string, string> defines{};

    //log
    bool bCreateLog{ false };
    string log{};
    string errorLog{};


    //loader values
    float loaderScale{ 1.0f };
    /*
    static const uint s_arbitaryValueCount = 10;

    //arbitary values
    uint fPointer{ s_arbitaryValueCount };
    float fValues[s_arbitaryValueCount] = {};

    float popFloat();
    float pushFloat();*/

  /*  //called after execution completed, mainly for logging?
    void verify();
    void appendToLog(string command, string message);
    void appendToError(string command, string message);
};*/

/*
class ConsoleCommand {

protected:
    string m_command;

public:
    ConsoleCommand(string command) : m_command(command) {}

    string const& getCommand() const { return m_command; }
    //returns a message with the command result
    virtual ExecutionResult execute(string args, ExecutionState& state) { return ExecutionResult(); }


    //does not process $defines 
    static string nextArgRaw(string& args);

    static string nextArg(string& args);

    static string nextPath(string& args);

    //try to parse nextBool
    static bool nextBool(string& args, bool& value);

    //try to parse next float
    static bool nextFloat(string& args, float& value);
};*/

