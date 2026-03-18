#pragma once
#include "debug_panel.h"

class GameObject;

class SceneHierarchy : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;

private:
	void drawSceneTree(GameObject* pGameObject);

};