#pragma once
#include "defines.h"

#include "gl_window.h"
#include "gl_shader.h"
#include "debug_ui.h"
#include "matrix.h"
#include "screen.h"
#include "graphics_context.h"

class Mesh;
class RenderShader;
class RenderNode;

class Renderer {
    friend class EngineFunctions;
    friend class ProjectTwo; //TO DO: remove later
    friend class ProjectThree;
    friend class ProjectFour;
public:

    Renderer() {}

    void init();
    void render();
    void shutdown();

    void handleResize(int width, int height);


private:

    vector<RenderNode*> m_renderables;
    void registerRenderable(RenderNode*);

    GraphicsContext m_graphicsContext{};
    Screen m_screen{ 0,0 };

};