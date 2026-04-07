#include "forward_render_pass.h"

#include "graphics_context.h"
#include "ngmath.h"
#include "graphics_library.h"
#include "render_node.h"

void ForwardRenderPass::executeRenderPass(GraphicsContext& context)
{
    GraphicsLibrary::faceCulling(false, true);
    GraphicsLibrary::depthTest(true);

    GraphicsLibrary::setViewport(0, 0, context.targetWidth(), context.targetHeight());
    GraphicsLibrary::setClearColor(m_clearColor);
    GraphicsLibrary::clear();
    GraphicsLibrary::clearDepth();

    
    for (auto it = m_renderables.begin(); it != m_renderables.end(); ++it) {
        (*it)->render(context);
    }

}
