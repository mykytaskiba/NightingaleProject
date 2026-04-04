#include "pch.h"
#include "window.h"
#include "graphics_library.h"
#include "engine_internals.h"
#include "engine_functions.h"
#include <filesystem>

void Window::init()
{
    //INIT GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_pWindow = createWindow(800, 600, "No Title Set");


    glfwSetFramebufferSizeCallback(m_pWindow, resizeCallback);
    glfwSetDropCallback(m_pWindow, dropCallback);


    //set initial size
    int width, height;
    glfwGetFramebufferSize(m_pWindow, &width, &height);
    resizeCallback(m_pWindow, width, height);

    //Turn off VSYNC
    glfwSwapInterval(0);
}

void Window::update()
{
    bool bShouldClose = glfwWindowShouldClose(m_pWindow);
    
    if (bShouldClose) {
        EngineFunctions::close_app();
        return;
    }

    glfwSwapBuffers(m_pWindow); 
    glfwPollEvents();

}

void Window::shutdown()
{
}

void Window::setTitle(std::string const& windowTitle)
{
    glfwSetWindowTitle(m_pWindow, windowTitle.c_str());
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
    EngineInternals::Renderer().handleResize(width, height);
}

void Window::dropCallback(GLFWwindow* window, int pathCount, const char** paths)
{
    for (unsigned int loop{ 0u }; loop < pathCount; ++loop) {
        std::filesystem::path file{ paths[loop] };
        EngineFunctions::engine().windowIO().dropCallback().execute(file);
    }

}

GLFWwindow* Window::createWindow(unsigned int width, unsigned int height, std::string const& title)
{
    //create window
    GLFWwindow* pWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (pWindow == NULL)
    {
        //TO DO: Replace cout with a log message
        std::cout << "Failed to create GLFW window" << std::endl;
        assert(false);
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(pWindow);
    glewInit();

    return pWindow;
}
