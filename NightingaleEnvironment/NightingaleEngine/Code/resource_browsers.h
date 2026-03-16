#pragma once
#include "debug_panel.h"

class ShaderResourceBrowser : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;
};

class MeshBrowser : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;
};