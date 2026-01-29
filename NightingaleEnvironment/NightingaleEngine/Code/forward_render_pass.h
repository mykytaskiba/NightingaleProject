#pragma once
#include "ngrender.h"

class ForwardRenderPass : public RenderPass {
public:
	void executeRenderPass(GraphicsContext& context) override;
};