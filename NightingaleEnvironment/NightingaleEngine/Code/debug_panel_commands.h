#pragma once
#include "command.h"

class RenderDebugCommand : public Command<bool> {
public:
    RenderDebugCommand() : Command<bool>("set_render_debug") {}
    virtual void execute_command(ArgumentList<bool>& args, ExecutionState& state, ExecutionResult& result) override;
};