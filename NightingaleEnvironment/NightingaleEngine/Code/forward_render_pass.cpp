#include "pch.h"
#include "forward_render_pass.h"

#include "gl_shader.h"
#include "screen.h"
#include "graphics_context.h"
#include "ngmath.h"
#include "graphics_library.h"
#include "render_node.h"

void ForwardRenderPass::executeRenderPass(GraphicsContext& context)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);


    GL::setViewport(0, 0, context.m_currentScreenDims.Width, context.m_currentScreenDims.Height);
    GL::setClearColor(Color(0.5, 0.5, 0.7, 1.0));
    GL::clear();
    GL::clearDepth();



    for (auto it = m_renderables.begin(); it != m_renderables.end(); ++it) {
        (*it)->render(context);
    }

}
