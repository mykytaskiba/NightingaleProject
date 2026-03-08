#pragma once
#include "game_object.h"
#include "defines.h"
#include "scripting.h"


class ConsoleCommand;

class Console {
    friend class HelpCommand;
    friend class ConsoleCommand;
public:
    void init();
    void tick();

protected:


    string m_consoleBuffer{};
    string m_consoleMessage{};

    vector<string> m_executedCommands{};
    size_t m_executedCommandBrowser{0u};

    bool m_consoleShown{ false }; //toggle by Tilda key
    uint m_consoleHeight = 300; //100 pixels?

};