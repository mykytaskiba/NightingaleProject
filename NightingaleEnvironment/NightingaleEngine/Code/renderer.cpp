#include "pch.h"

#include "renderer.h"
#include "graphics_library.h"
#include "shader_source.h"
#include "render_node.h"
#include "ngrender.h"

//TEMP INCLUDES
#include "game_object.h"
#include "render_shader.h"
#include "termination.h"
#include "loader.h"
#include "mesh.h"
#include "input.h"
#include "engine_internals.h"
#include "ngmath.h"
#include "forward_render_pass.h"

//TO DO remove this temp include
#include "skeleton.h"



void Renderer::init()
{
    m_graphicsContext.init();

    registerRenderPass(new ForwardRenderPass());
}

void Renderer::render()
{


    for (auto it{ m_renderpasses.begin() }; it != m_renderpasses.end(); ++it) {
        (*it)->executeRenderPass(m_graphicsContext);
    }

    m_graphicsContext.endFrame();


}


void Renderer::handleResize(int width, int height)
{
    m_screen.Width = width;
    m_screen.Height = height;

    m_graphicsContext.m_currentScreenDims.Width = width;
    m_graphicsContext.m_currentScreenDims.Height = height;

    EngineInternals::Camera().SetTargetSize(width, height);
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
