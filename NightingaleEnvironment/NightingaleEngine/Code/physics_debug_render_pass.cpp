#include "pch.h"
#include "physics_debug_render_pass.h"
#include "imgui_helpers.h"
#include "ngrender.h"


void PhysicsDebugRenderPass::executeRenderPass(GraphicsContext& context)
{
    if (!m_bDepthTest) {
        glDisable(GL_DEPTH_TEST);
    }
    else {
        glEnable(GL_DEPTH_TEST);
    }


	for (PhysicsBody* pBody : m_physics.m_vActiveBodies) {
		//context.drawAxisAlignedBox(pBody->getGlobalBox(), m_AABBColor);
        assert(false); //THIS IS IMPORTANT IMPLEMENT THIS
	}
}

void PhysicsDebugRenderPass::registerRenderable(RenderNode* renderable)
{
}

