#include "graphics_library.h"
#include <GL/glew.h>


void GraphicsLibrary::setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
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

void GraphicsLibrary::faceCulling(bool bCullFront, bool bCullBack)
{
    if (!bCullFront && !bCullBack) {
        glDisable(GL_CULL_FACE);
        return;
    }


    GLenum mode = GL_FRONT_AND_BACK;
    if (!bCullFront) {
        mode = GL_BACK;
    }
    else if (!bCullBack) {
        mode = GL_FRONT;
    }

    glEnable(GL_CULL_FACE);
    glCullFace(mode);
}

void GraphicsLibrary::depthTest(bool bEnabled)
{
    if (bEnabled) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
}
