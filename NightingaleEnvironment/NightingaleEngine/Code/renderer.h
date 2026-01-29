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


private:

    vector<RenderPass*> m_renderpasses;
    void registerRenderPass(RenderPass*);

    vector<RenderNode*> m_renderables;
    void registerRenderable(RenderNode*);

    GraphicsContext m_graphicsContext{};
    Screen m_screen{ 0,0 };

};