#pragma once
#include "render_pass.h"

class ForwardRenderPass : public RenderPass {
public:
	void executeRenderPass(GraphicsContext& context) override;
};