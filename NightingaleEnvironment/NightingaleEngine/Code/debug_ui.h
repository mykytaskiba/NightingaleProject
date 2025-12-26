#pragma once

class Window;

class DebugUI {
public:

    DebugUI();

    void init(Window&);
    void shutdown();

    void newFrame();
    void endFrame();


};