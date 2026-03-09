#include "pch.h"
#include "debug_panel_commands.h"
#include "engine_functions.h"

void RenderPanelCommand::execute_command(ArgumentList<Nothing>& args, ExecutionState& state, ExecutionResult& result)
{
    m_panel.toggle();
}
