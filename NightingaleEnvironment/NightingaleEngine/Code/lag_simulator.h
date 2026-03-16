#pragma once
#include "debug_panel.h"

class LagSimulatorPanel : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;

private:
	float m_lagTimeSeconds{ 1.0f };
};
