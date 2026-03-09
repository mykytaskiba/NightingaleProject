#pragma once
#include "debug_panel.h"
#include "ngrender.h"

class RenderDebugPanel : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;


	void render_pass_debug(RenderPass* pRenderPass);
};