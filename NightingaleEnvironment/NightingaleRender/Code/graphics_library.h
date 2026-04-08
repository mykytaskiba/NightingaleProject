#pragma once
#include "color.h"

/// <summary>
/// Abstraction of all opengl/graphic library calls through one class
/// </summary>
class GraphicsLibrary {
public:

    static Color s_clearColor;

    static void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height);

    static void clear(bool bDepth, bool bColor, Color const& color, bool bStencil = false);

    
    static void faceCulling(bool bCullFront = false, bool bCullBack = false);

    static void depthTest(bool bEnabled);

};
