#include "render_pass.h"

void RenderPass::registerRenderable(RenderNode* renderable)
{
	m_renderables.push_back(renderable);
}

void RenderPass::removeRenderable(RenderNode* pRenderNode)
{
	auto it = std::find(m_renderables.begin(), m_renderables.end(), pRenderNode);
	if (it == m_renderables.end()) {
		return;
	}
	m_renderables.erase(it);
}
