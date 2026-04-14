#include "pch.h"

#include "renderer.h"
#include "shader_source.h"
#include "ngrender.h"

//TEMP INCLUDES
#include "game_object.h"
#include "loader.h"
#include "input.h"
#include "engine_internals.h"
#include "ngmath.h"
#include "engine_functions.h"

//TO DO remove this temp include
#include "skeleton.h"



void Renderer::init()
{

    registerRenderPass(new ForwardRenderPass());

    m_graphicsContext.primitives().setCubeMesh(AssetManager<Mesh>::retrieve("cube_mesh"));
    m_graphicsContext.primitives().setSphereMesh(AssetManager<Mesh>::retrieve("sphere_mesh"));
    m_graphicsContext.primitives().setUnlitShader(AssetManager<RenderShader>::retrieve("unlit_shader"));
    m_graphicsContext.primitives().setLineShader(AssetManager<RenderShader>::retrieve("line_shader"));
}

void Renderer::render()
{
    
    if (m_renderpasses.empty()) {
        //fallback for a no render pass moment, just draw black screen
        GraphicsLibrary::clear(false, true, Color{ 0.0f,0.0f,0.0f,1.0f });
        return;
    }

    m_graphicsContext.beginFrame();

    m_graphicsContext.setCamera(&EngineFunctions::scene().getMainCamera());

    for (RenderPass* pRenderPass : m_renderpasses) {
        if (!pRenderPass->isEnabled()) {
            continue;
        }
        pRenderPass->executeRenderPass(m_graphicsContext);
    }

    m_graphicsContext.endFrame();


}


void Renderer::handleResize(int width, int height)
{
    m_graphicsContext.setTargetDimensions(width, height);
}

void Renderer::registerRenderPass(RenderPass* pRenderPass)
{
    m_renderpasses.push_back(pRenderPass);

    for (auto it{ m_renderables.begin() }; it != m_renderables.end(); ++it) {
        pRenderPass->registerRenderable(*it);
    }
}

void Renderer::registerRenderable(RenderNode* pRenderNode)
{
    m_renderables.push_back(pRenderNode);

    for (auto it{ m_renderpasses.begin() }; it != m_renderpasses.end(); ++it) {
        (*it)->registerRenderable(pRenderNode);
    }
}

void Renderer::removeRenderPass(RenderPass*& pRenderPass)
{
    m_renderpasses.erase(std::remove(m_renderpasses.begin(), m_renderpasses.end(), pRenderPass), m_renderpasses.end());
    delete pRenderPass;

    pRenderPass = nullptr;
}

void Renderer::clearAllRenderPasses()
{
    for (RenderPass* pRenderPass : m_renderpasses) {
        delete pRenderPass;
    }
    m_renderpasses.clear();
}

void Renderer::removeRenderable(RenderNode* pRenderNode)
{
    auto itFound = std::find(m_renderables.begin(), m_renderables.end(), pRenderNode);
    if (itFound == m_renderables.end()) {
        assert(false);
        return;
    }

    m_renderables.erase(itFound);

    for (RenderPass* pRenderPass : m_renderpasses) {
        if (pRenderPass == nullptr) {
            assert(false);
            continue;
        }
        pRenderPass->removeRenderable(pRenderNode);
    }
}
