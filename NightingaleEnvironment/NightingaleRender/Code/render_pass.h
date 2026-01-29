#pragma once
#include "defines.h"

class GraphicsContext;
class RenderNode;

class RenderPass {

public:

	virtual void executeRenderPass(GraphicsContext& context) = 0;

	//by default each render pass is interested in all the nodes
	virtual void registerRenderable(RenderNode* renderable);
protected:


	//render nodes of interest for this specific pass
	vector<RenderNode*> m_renderables;

	

};