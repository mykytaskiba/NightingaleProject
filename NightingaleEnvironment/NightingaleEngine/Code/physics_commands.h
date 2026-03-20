#pragma once
#include "command.h"

class PhysicsActiveCommand : public Command<TriBool> {
public:
    PhysicsActiveCommand() : Command<TriBool>("physics_active") {}
    virtual void execute_command(ArgumentList<TriBool>& args, ExecutionState& state, ExecutionResult& result) override;
};

//attach a single physics body to selected guid object
class AttachPhysicsBodyCommand : public Command<SelectedGameObject> {
public:
    AttachPhysicsBodyCommand() : Command("attach_physicsbody") {}
    virtual void execute_command(ArgumentList<SelectedGameObject>& args, ExecutionState& state, ExecutionResult& result) override;
};