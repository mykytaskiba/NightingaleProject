#pragma once
#include "command.h"
#include "callback_handler.h"
#include "render_debug_panel.h"

class RenderDebugCommand : public Command<bool> {
public:
    RenderDebugCommand() : Command<bool>("set_render_debug") {}
    virtual void execute_command(ArgumentList<bool>& args, ExecutionState& state, ExecutionResult& result) override;

    bool m_bActive{ false };
    CallbackRef m_updateLoopRef{};
    RenderDebugPanel m_panel;
};