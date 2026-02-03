#pragma once
#include "defines.h"

#include "gl_shader.h"
#include "debug_ui.h"
#include "screen.h"
#include "graphics_context.h"
#include "ngmath.h"

class Mesh;
class RenderShader;
class RenderNode;
class RenderPass;

class Renderer {
    friend class EngineFunctions;
    friend class ProjectTwo; //TO DO: remove later
    friend class ProjectThree;
    friend class ProjectFour;
public:

    Renderer() {}

    void init();
    void render();

    void handleResize(int width, int height);

    void registerRenderPass(RenderPass*);
    void registerRenderable(RenderNode*);

private:

    vector<RenderPass*> m_renderpasses;

    vector<RenderNode*> m_renderables;

    GraphicsContext m_graphicsContext{};
    Screen m_screen{ 0,0 };

};