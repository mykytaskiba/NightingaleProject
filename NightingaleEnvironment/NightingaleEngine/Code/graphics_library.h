#pragma once

#include "ngrender.h"
#include "defines.h"
#include "ngmath.h"

/// <summary>
/// Abstraction of all opengl/graphic library calls through one class
/// </summary>
class GraphicsLibrary {
public:


    static void setViewport(uint x, uint y, uint width, uint height);

    static void setClearColor(Color const& color);
    static void clear();

    static void clearDepth();



private:
};

using GL = GraphicsLibrary;