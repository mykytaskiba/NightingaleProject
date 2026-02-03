#include "pch.h"
#include "physics_debug_render_pass.h"
#include "graphics_context.h"


void PhysicsDebugRenderPass::executeRenderPass(GraphicsContext& context)
{
	for (Body* pBody : m_physics.m_activeBodies) {
		context.drawAxisAlignedBox(pBody->globalBox);
	}
}

void PhysicsDebugRenderPass::registerRenderable(RenderNode* renderable)
{
}
