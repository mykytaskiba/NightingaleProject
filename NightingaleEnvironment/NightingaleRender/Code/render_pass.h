#pragma once
#include "defines.h"

class GraphicsContext;
class RenderNode;

class RenderPass {

public:

	virtual void executeRenderPass(GraphicsContext& context) = 0;

	//by default each render pass is interested in all the nodes
	virtual void registerRenderable(RenderNode* renderable);

	bool isEnabled() { return m_enabled; };

	virtual void debugUIFunction() = 0;
protected:

	//whether this pass is enabled
	bool m_enabled{ true };


	//render nodes of interest for this specific pass
	vector<RenderNode*> m_renderables;

	

};