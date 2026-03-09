#pragma once
#include "command.h"
#include "callback_handler.h"
#include "render_debug_panel.h"

class RenderPanelCommand : public Command<Nothing> {
public:
    RenderPanelCommand() : Command<Nothing>("render_panel") {}
    virtual void execute_command(ArgumentList<Nothing>& args, ExecutionState& state, ExecutionResult& result) override;
    RenderDebugPanel m_panel;
};