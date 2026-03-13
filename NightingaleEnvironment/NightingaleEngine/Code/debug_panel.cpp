#include "pch.h"
#include "debug_panel.h"
#include "engine_internals.h"
#include "engine_functions.h"

void DebugPanel::toggle()
{
    bool bNextState = !m_bActive;

    if (bNextState) {
        //Switching to active
        activate();

        m_updateLoopRef = EngineFunctions::getUpdateCallback().addCallback(
            {
                110,
                [this] { render_update(); }
            }
        );

    }
    else {
        //Switching to inactive
        deactivate();
        EngineFunctions::getUpdateCallback().removeCallback(m_updateLoopRef);
    }

    m_bActive = bNextState;

}

void DebugPanel::toggle_set(bool bState)
{
    if (bState != m_bActive) {
        toggle();
    }
}
