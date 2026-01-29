#pragma once
#include "engine.h"

class GLFWWindow;

static class EngineInternals {
private:

    friend class Engine;
    friend class Window;
    friend class Renderer;
    friend class EngineFunctions;
    friend class ConsoleCommand;
    friend class GraphicsContext;

    friend class ProjectTwo; //TO DO: temoporary hack
    friend class ProjectThree;
    friend class ProjectFour;

    static Engine* pEngine;

    static void init(Engine& engine);

    static Renderer& Renderer();

    static Scene& Scene();

    static Camera& Camera();

public:


};