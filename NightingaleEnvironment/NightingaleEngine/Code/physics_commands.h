#pragma once
#include "command.h"

class PhysicsActiveCommand : public Command<TriBool> {
public:
    PhysicsActiveCommand() : Command<TriBool>("physics_active") {}
    virtual void execute_command(ArgumentList<TriBool>& args, ExecutionState& state, ExecutionResult& result) override;
};