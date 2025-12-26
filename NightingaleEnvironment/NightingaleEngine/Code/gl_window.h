#pragma once

#include "defines.h"

struct GLFWwindow;

class GL_Window {

    friend class GraphicsLibrary;
    friend class DebugUI;
    friend class Window;
private:
    GLFWwindow* pWindow;

    bool isInitialized() { return (pWindow == nullptr); }

public:
    GL_Window(GLFWwindow* pWindowIn) : pWindow(pWindowIn) {}
    GL_Window() : pWindow(nullptr) {}
};