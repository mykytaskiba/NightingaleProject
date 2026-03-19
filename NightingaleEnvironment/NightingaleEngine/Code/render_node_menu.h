#pragma once
#include "ngrender.h"

class RenderMeshNode;
class GameObject;

class RenderNodeMenu {
public:
	static void render_update(GameObject& gameObject);
	static void render_update(RenderMeshNode* pRenderMesh);
};