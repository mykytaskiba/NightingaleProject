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

        if (m_bDrawCollisionPoints) {
            for (auto const& collisionPoint : m_physics.collisionPoints) {
                context.primitives().drawSphere(collisionPoint.point, 0.1f, m_collisionPointsColor);
                context.primitives().drawLine(collisionPoint.point, collisionPoint.force.normalized() * 1.5f + collisionPoint.point, m_collisionPointsColor);
            }
        }

        if (m_bCollisionMeshActive && pBody->getShape() != nullptr) {
            SphereShape* pSphere = dynamic_cast<SphereShape*>(pBody->getShape());
            if (pSphere != nullptr) {
                context.primitives().drawWireSphere(pBody->getPosition(), pSphere->getRadius(), m_collisionMeshColor);
                continue;
            }

            BoxShape* pBox = dynamic_cast<BoxShape*>(pBody->getShape());
            if (pBox != nullptr) {
                Vector3 axis1;
                Vector3 axis2;
                Vector3 axis3;
                pBox->getAxisFromRotation(pBody->getRotation(), axis1, axis2, axis3);

                context.primitives().drawOrientedBox(pBody->getPosition(), pBox->getHalfSize(), axis1, axis2, axis3, m_collisionMeshColor);
                continue;
            }
        }

    }
}

void PhysicsDebugRenderPass::registerRenderable(RenderNode* renderable)
{
}

