#pragma once
#include "color.h"

/// <summary>
/// Abstraction of all opengl/graphic library calls through one class
/// </summary>
class GraphicsLibrary {
public:


    static void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    static void setClearColor(Color const& color);
    static void clear();

    static void clearDepth();

    static void faceCulling(bool bCullFront = false, bool bCullBack = false);

    static void depthTest(bool bEnabled);

};
