#include "pch.h"
#include "physics_debug_render_pass.h"
#include "imgui_helpers.h"
#include "ngrender.h"
#include "ngphys.h"

void PhysicsDebugRenderPass::executeRenderPass(GraphicsContext& context)
{
    GraphicsLibrary::depthTest(m_bDepthTest);

    for (PhysicsBody* pBody : m_physics.m_vActiveBodies) {
        if (m_bAxisAlignedBoxesActive) {
            context.primitives().drawAxisAlignedBox(pBody->getGlobalBox(), m_axisAlignedBoxesColor);
        }

        if (m_bVelocityArrowActive) {
            context.primitives().drawLine(pBody->getPosition(), pBody->getPosition() + pBody->getVelocity(), m_velocityArrowColor);
        }

        if (pBody->getShape() != nullptr) {
            SphereShape* pSphere = dynamic_cast<SphereShape*>(pBody->getShape());
            if (pSphere != nullptr) {
                context.primitives().drawWireSphere(pBody->getPosition(), pSphere->getRadius());
            }
        }
    }
}

void PhysicsDebugRenderPass::registerRenderable(RenderNode* renderable)
{
}

