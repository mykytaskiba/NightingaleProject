#pragma once
#include <functional>
#include <vector>
#include <string>

class GLFWwindow;

//TO DO: Potentially make this a wrapper and a Window_GLFW a specific implementation?
class Window {
public:
    void init();
    void update();
    void shutdown();

    void setTitle(std::string const& windowTitle);

    GLFWwindow* GetNativeWindow() { return m_pWindow; }

private:

    static void resizeCallback(GLFWwindow* window, int width, int height);
    static void dropCallback(GLFWwindow* window, int pathCount, const char** paths);


    static GLFWwindow* createWindow(unsigned int width, unsigned int height, std::string const& title);

    GLFWwindow* m_pWindow;
};