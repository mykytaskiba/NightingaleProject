#pragma once
#include "debug_panel.h"

class SceneHierarchy : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;

};