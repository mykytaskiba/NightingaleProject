#pragma once
#include "game_object.h"
#include "defines.h"
#include "console_command.h"
#include "afterparty.h"


class ConsoleCommand;

class Console {
    friend class HelpCommand;
    friend class ConsoleCommand;
public:
    virtual void init();
    virtual void tick();
    virtual void shutdown();
protected:


    string m_consoleBuffer{};
    string m_consoleMessage{};

    vector<string> m_executedCommands{};
    size_t m_executedCommandBrowser{0u};

    bool m_consoleShown{ false }; //toggle by Tilda key
    uint m_consoleHeight = 300; //100 pixels?

};