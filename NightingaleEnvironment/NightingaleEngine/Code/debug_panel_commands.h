#pragma once
#include "command.h"
#include "callback_handler.h"
#include "render_debug_panel.h"
#include "engine_menu_bar.h"


template <typename TPanel> requires std::derived_from<TPanel, DebugPanel>
class PanelCommand : public Command<TriBool> {
public:
    PanelCommand(std::string command) : Command<TriBool>(command) {};

    virtual void execute_command(ArgumentList<TriBool>& args, ExecutionState& state, ExecutionResult& result) override {
        if (args.get<0>() == TriBool::TOGGLE) {
            m_panel.toggle();
        }
        if (args.get<0>() == TriBool::TRUE) {
            m_panel.toggle_set(true);
        }
        if (args.get<0>() == TriBool::FALSE) {
            m_panel.toggle_set(false);
        }
    }
    
protected:
    TPanel m_panel;
};


class EngineMenuCommand : public PanelCommand<EngineMenuBar> {
public:
    EngineMenuCommand() : PanelCommand<EngineMenuBar>("engine_menu") {}
};

class RenderPanelCommand : public PanelCommand<RenderDebugPanel> {
public:
    RenderPanelCommand() : PanelCommand<RenderDebugPanel>("render_panel") {}
};
