#pragma once
#include "gl_window.h"
#include "defines.h"

class Window {
public:
    void init();
    void update();
    void shutdown();

    void setTitle(string const& windowTitle);

    GLFWwindow* GetNativeWindow() { return m_window.pWindow; }
private:

    static void resizeCallback(GLFWwindow* window, int width, int height);
    static void dropCallback(GLFWwindow* window, int pathCount, const char** paths);

    GL_Window m_window;
};