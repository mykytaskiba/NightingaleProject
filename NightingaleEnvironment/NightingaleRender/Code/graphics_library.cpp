#include "graphics_library.h"
#include <GL/glew.h>
#include "nightingale_assert.h"


Color GraphicsLibrary::s_clearColor = Color{ -1.0f,-1.0f,-1.0f,-1.0f };

void GraphicsLibrary::setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) {
    glViewport(x, y, width, height);
}

void GraphicsLibrary::clear(bool bDepth, bool bColor, Color const& color, bool bStencil)
{
    if (!bDepth && !bColor && !bStencil) {
        return;
    }

    if (bColor && color != s_clearColor) {
        glClearColor(color.r, color.g, color.b, color.a);
        s_clearColor = color;
    }
    GLbitfield clearMask = 0;
    if (bColor) clearMask |= GL_COLOR_BUFFER_BIT;
    if (bDepth) clearMask |= GL_DEPTH_BUFFER_BIT;
    if (bStencil) clearMask |= GL_STENCIL_BUFFER_BIT;

    glClear(clearMask);
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
