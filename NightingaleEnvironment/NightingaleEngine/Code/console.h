#pragma once
#include "game_object.h"
#include "defines.h"
#include "scripting.h"
#include "debug_panel.h"

class ConsoleCommand;

class Console : public DebugPanel {
    friend class HelpCommand;
    friend class ConsoleCommand;
public:
    void activate() override;
    void deactivate() override {};
    void render_update() override;

protected:


    string m_consoleBuffer{};
    string m_consoleMessage{};

    vector<string> m_executedCommands{};
    size_t m_executedCommandBrowser{0u};

    bool m_bShownThisFrame{ false };

};