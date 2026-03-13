#pragma once

#include "ngphys.h"
#include "ngrender.h"

class PhysicsDebugRenderPass : public RenderPass {

public:

	PhysicsDebugRenderPass(Physics& physics) : m_physics(physics) {}

	Physics& m_physics;

	bool m_bDepthTest{ false };
	Color m_AABBColor{ 0.627f, 0.0f,0.627f,1.0f };

	void executeRenderPass(GraphicsContext& context) override;

	void registerRenderable(RenderNode* renderable) override;

	void debugUIFunction() override;
};