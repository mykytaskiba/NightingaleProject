#pragma once
#include "debug_panel.h"

class EngineMenuBar : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;
};

class ImGUIDemoPanel : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;
};