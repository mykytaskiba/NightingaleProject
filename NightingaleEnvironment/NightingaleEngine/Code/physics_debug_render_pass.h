#pragma once

#include "ngphys.h"
#include "ngrender.h"

class PhysicsDebugRenderPass : public RenderPass {

public:

	PhysicsDebugRenderPass(Physics& physics) : m_physics(physics) {}

	Physics& m_physics;

	void executeRenderPass(GraphicsContext& context) override;

	void registerRenderable(RenderNode* renderable) override;

	void debugUIFunction() override;
};