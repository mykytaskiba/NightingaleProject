#include "pch.h"
#include "forward_render_pass.h"

#include "ngrender.h"
#include "screen.h"
#include "graphics_context.h"
#include "ngmath.h"
#include "graphics_library.h"
#include "render_node.h"
#include "imgui_helpers.h"

void ForwardRenderPass::executeRenderPass(GraphicsContext& context)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);


    GL::setViewport(0, 0, context.m_currentScreenDims.Width, context.m_currentScreenDims.Height);
    GL::setClearColor(m_clearColor);
    GL::clear();
    GL::clearDepth();

    for (auto it = m_renderables.begin(); it != m_renderables.end(); ++it) {
        (*it)->render(context);
    }

}

void ForwardRenderPass::debugUIFunction()
{


    
    ImGui::Checkbox("Enabled", &m_enabled);

    if (ImGui::CollapsingHeader("Clear Color")) {
        ImGuiHelpers::ColorPicker3("",m_clearColor);
    }


}
