#pragma once
#include "ngrender.h"

class ForwardRenderPass : public RenderPass {

private:
	Color m_clearColor{ 0.5, 0.5, 0.7, 1.0 };
public:

	ForwardRenderPass() : RenderPass("Forward Render Pass") {}

	void executeRenderPass(GraphicsContext& context) override;
	void debugUIFunction() override;
};