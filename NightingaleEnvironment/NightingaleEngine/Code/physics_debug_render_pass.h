#pragma once

#include "ngphys.h"
#include "ngrender.h"
#include "color.h"

class PhysicsDebugRenderPass : public RenderPass {

public:

	PhysicsDebugRenderPass(Physics& physics) : RenderPass("Physics Debug Pass"), m_physics(physics) {}

	Physics& m_physics;

	bool m_bDepthTest{ false };

	bool m_bAxisAlignedBoxesActive{ true };
	Color m_axisAlignedBoxesColor{ 0.627f, 0.0f,0.627f,1.0f };

	bool m_bVelocityArrowActive{ true };
	Color m_velocityArrowColor{ 0.0f, 0.9f,0.9f, 1.0f };

	bool m_bCollisionMeshActive{ true };
	Color m_collisionMeshColor{ 0.0f, 0.75f, 0.0f, 1.0f };

	bool m_bDrawCollisionPoints{ true };
	Color m_collisionPointsColor{ 0.2f, 0.2f, 0.85f, 1.0f };

	void executeRenderPass(GraphicsContext& context) override;

	void registerRenderable(RenderNode* renderable) override;

	void properties(IPropertyVisitor& visitor) override {
		visitor("depth_test",m_bDepthTest);
		visitor("aabb_active", m_bAxisAlignedBoxesActive);
		visitor("aabb_color", m_axisAlignedBoxesColor);
		visitor("velocity_arrow_active", m_bVelocityArrowActive);
		visitor("velocity_arrow_color", m_velocityArrowColor);
		visitor("collision_mesh_active", m_bCollisionMeshActive);
		visitor("collision_mesh_color", m_collisionMeshColor);
		visitor("collision_points_active", m_bDrawCollisionPoints);
		visitor("collision_points_color", m_collisionPointsColor);
	}

};