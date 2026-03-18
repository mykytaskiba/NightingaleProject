#pragma once
#include "debug_panel.h"
#include "guid.h"

class GameObject;

class SceneHierarchy : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;

private:
	
	void drawSceneTree(GameObject* pGameObject);

	bool m_bShowGUID{ true };

};

class Inspector : public DebugPanel {
public:
	void activate() override {};
	void deactivate() override {};
	void render_update() override;
};