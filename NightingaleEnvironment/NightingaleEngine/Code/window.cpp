#include "pch.h"
#include "window.h"
#include "graphics_library.h"
#include "termination.h"
#include "engine_internals.h"

void Window::init()
{
    GL::init();

    m_window = GL::createWindow(800, 600, "No Title Set");

    glfwSetFramebufferSizeCallback(m_window.pWindow, resizeCallback);

    glfwSetDropCallback(m_window.pWindow, dropCallback);


    //set initial size
    int width, height;
    glfwGetFramebufferSize(m_window.pWindow, &width, &height);
    resizeCallback(m_window.pWindow, width, height);

}

void Window::update()
{
    if (GL::shouldCloseWindow(m_window)) {
        Termination::SendTerminationSignal();
        return;
    }
    GL::swapBuffers(m_window);
    GL::pollEvents();

}

void Window::shutdown()
{
}

void Window::setTitle(string const& windowTitle)
{
    glfwSetWindowTitle(m_window.pWindow, windowTitle.c_str());
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
    EngineInternals::Renderer().handleResize(width, height);
}

void Window::dropCallback(GLFWwindow* window, int pathCount, const char** paths)
{
    if (pathCount != 1) return;

}
