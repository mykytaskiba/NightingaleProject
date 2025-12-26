#pragma once

#include "gl_window.h"
#include "gl_shader.h"
#include "defines.h"
#include "color.h"

/// <summary>
/// Abstraction of all opengl/graphic library calls through one class
/// </summary>
class GraphicsLibrary {
public:
    static void init();

    static GL_Window createWindow(uint width, uint height, string const& title);
    static bool shouldCloseWindow(GL_Window&);

    static void setViewport(uint x, uint y, uint width, uint height);

    static void setClearColor(Color const& color);
    static void clear();

    static void clearDepth();

    static void swapBuffers(GL_Window&);
    static void pollEvents();


private:
};

using GL = GraphicsLibrary;