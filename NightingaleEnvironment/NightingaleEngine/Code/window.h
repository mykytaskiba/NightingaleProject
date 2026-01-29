#pragma once
#include "gl_window.h"
#include "defines.h"

class GLFWwindow;

class Window {
public:
    void init();
    void update();
    void shutdown();

    void setTitle(string const& windowTitle);

    GLFWwindow* GetNativeWindow() { return m_pWindow; }
private:

    static void resizeCallback(GLFWwindow* window, int width, int height);
    static void dropCallback(GLFWwindow* window, int pathCount, const char** paths);


    static GLFWwindow* createWindow(uint width, uint height, string const& title);

    GLFWwindow* m_pWindow;
};