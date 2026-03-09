#include "pch.h"
#include "physics_debug_render_pass.h"
#include "graphics_context.h"


void PhysicsDebugRenderPass::executeRenderPass(GraphicsContext& context)
{
	glDisable(GL_DEPTH_TEST);
	for (PhysicsBody* pBody : m_physics.m_activeBodies) {
		context.drawAxisAlignedBox(pBody->globalBox);
	}
	glEnable(GL_DEPTH_TEST);
}

void PhysicsDebugRenderPass::registerRenderable(RenderNode* renderable)
{
}

void PhysicsDebugRenderPass::debugUIFunction()
{
	ImGui::Text("Physics Debug Pass");
}
