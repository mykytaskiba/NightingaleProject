#include "pch.h"
#include "graphics_library.h"

void GraphicsLibrary::init()
{
    //INIT GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

}

GL_Window GraphicsLibrary::createWindow(uint width, uint height, string const& title) {
    //create window
    GLFWwindow* pWindow = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
    if (pWindow == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return GL_Window(nullptr);
    }
    
    glfwMakeContextCurrent(pWindow);
    glewInit();


    return GL_Window(pWindow);
}

bool GraphicsLibrary::shouldCloseWindow(GL_Window& window) {
    return glfwWindowShouldClose(window.pWindow);
}

void GraphicsLibrary::setViewport(uint x, uint y, uint width, uint height) {
    glViewport(x, y, width, height);
}

void GraphicsLibrary::setClearColor(Color const& color) {
    glClearColor(color.r, color.g, color.b, color.a);
}
void GraphicsLibrary::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}
void GraphicsLibrary::clearDepth() {
    glClear(GL_DEPTH_BUFFER_BIT);
}

void GraphicsLibrary::swapBuffers(GL_Window& window) {
    glfwSwapBuffers(window.pWindow);
}

void GraphicsLibrary::pollEvents() {
    glfwPollEvents();
}