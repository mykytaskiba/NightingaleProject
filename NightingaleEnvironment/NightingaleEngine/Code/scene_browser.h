#pragma once
#include "debug_panel.h"

class SceneBrowser : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;

};