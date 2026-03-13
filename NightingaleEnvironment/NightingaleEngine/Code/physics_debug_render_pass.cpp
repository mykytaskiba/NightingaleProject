#include "pch.h"
#include "physics_debug_render_pass.h"
#include "graphics_context.h"
#include "imgui_helpers.h"


void PhysicsDebugRenderPass::executeRenderPass(GraphicsContext& context)
{
	glDisable(GL_DEPTH_TEST);
	for (PhysicsBody* pBody : m_physics.m_activeBodies) {
		context.drawAxisAlignedBox(pBody->globalBox, m_AABBColor);
	}
	glEnable(GL_DEPTH_TEST);
}

void PhysicsDebugRenderPass::registerRenderable(RenderNode* renderable)
{
}

void PhysicsDebugRenderPass::debugUIFunction()
{
    if (!ImGui::CollapsingHeader("Physics Debug Pass")) return;

    ImGui::Checkbox("Enabled", &m_enabled);

    float width = ImGui::GetContentRegionAvail().x;
    ImGui::BeginChild("PHYSICS_DEBUG_PASS", ImVec2(width, 0), ImGuiChildFlags_AutoResizeY | ImGuiChildFlags_Border, ImGuiWindowFlags_MenuBar);


    if (ImGui::CollapsingHeader("AABB Color")) {
        ImGuiHelpers::ColorPicker3("", m_AABBColor);
    }

    ImGui::EndChild();
}
