#include "render_pass.h"

void RenderPass::registerRenderable(RenderNode* renderable)
{
	m_renderables.push_back(renderable);
}
