#include "pch.h"
#include "debug_panel_commands.h"
#include "engine_functions.h"

void RenderDebugCommand::execute_command(ArgumentList<bool>& args, ExecutionState& state, ExecutionResult& result)
{
    bool bOldState = m_bActive;
    bool bNewState = args.get<0>();

    if (bOldState == bNewState) {
        result.bSuccess = true;
        return;
    }

    if (bNewState) {
        //Switching to active
        m_panel.activate();

        m_updateLoopRef = EngineFunctions::getUpdateCallback().addCallback(
            { 
                110, 
                [this] { m_panel.render_update(); }    
            }
        );

    }
    else {
        //Switching to inactive
        m_panel.deactivate();
        EngineFunctions::getUpdateCallback().removeCallback(m_updateLoopRef);
    }

    m_bActive = bNewState;

}
