#include "pch.h"
#include "graphics_library.h"

void GraphicsLibrary::init()
{

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
