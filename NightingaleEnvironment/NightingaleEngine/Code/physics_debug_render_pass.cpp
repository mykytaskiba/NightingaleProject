#include "pch.h"
#include "physics_debug_render_pass.h"
#include "graphics_context.h"
#include "imgui_helpers.h"


void PhysicsDebugRenderPass::executeRenderPass(GraphicsContext& context)
{
    if (!m_bDepthTest) {
        glDisable(GL_DEPTH_TEST);
    }
    else {
        glEnable(GL_DEPTH_TEST);
    }


	for (PhysicsBody* pBody : m_physics.m_activeBodies) {
		context.drawAxisAlignedBox(pBody->globalBox, m_AABBColor);
	}
}

void PhysicsDebugRenderPass::registerRenderable(RenderNode* renderable)
{
}

void PhysicsDebugRenderPass::debugUIFunction()
{

    ImGui::Checkbox("Enabled", &m_enabled);
    ImGui::Checkbox("Depth Test", &m_bDepthTest);


    if (ImGui::CollapsingHeader("AABB Color")) {
        ImGuiHelpers::ColorPicker3("", m_AABBColor);
    }
}
