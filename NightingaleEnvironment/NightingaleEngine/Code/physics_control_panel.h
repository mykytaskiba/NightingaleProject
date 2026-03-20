#pragma once
#include "debug_panel.h"

class PhysicsDebugRenderPass;

class PhysicsControlPanel : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;

private:
	PhysicsDebugRenderPass* findDebugPass();

	void setTestCase();
};