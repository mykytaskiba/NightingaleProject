#include "pch.h"

#include "renderer.h"
#include "graphics_library.h"
#include "shader_source.h"
#include "render_node.h"

//TEMP INCLUDES
#include "game_object.h"
#include "vector.h"
#include "matrix.h"
#include "render_shader.h"
#include "termination.h"
#include "loader.h"
#include "mesh.h"
#include "input.h"
#include "engine_internals.h"

//TO DO remove this temp include
#include "skeleton.h"


void Renderer::init()
{
    m_graphicsContext.init();
    
}

void Renderer::render()
{



    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);


    GL::setViewport(0, 0, m_screen.Width, m_screen.Height);
    GL::setClearColor(Color(0.5, 0.5, 0.7, 1.0));
    GL::clear();
    GL::clearDepth();

    

    for (auto it = m_renderables.begin(); it != m_renderables.end(); ++it) {
        (*it)->render(m_graphicsContext);
    }

    m_graphicsContext.endFrame();


}

void Renderer::shutdown()
{
}

void Renderer::handleResize(int width, int height)
{
    m_screen.Width = width;
    m_screen.Height = height;

    EngineInternals::Camera().SetTargetSize(width, height);
}

void Renderer::registerRenderable(RenderNode* pRenderNode)
{
    m_renderables.push_back(pRenderNode);
}
