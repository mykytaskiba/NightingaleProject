#include "pch.h"
#include "engine_internals.h"
#include "engine.h"

Engine* EngineInternals::pEngine = nullptr;

void EngineInternals::init(Engine& engine)
{
    pEngine = &engine;
}

Renderer& EngineInternals::Renderer()
{
    assert(pEngine != nullptr);
    return pEngine->m_renderer;
}

Scene& EngineInternals::Scene()
{
    assert(pEngine != nullptr);
    return pEngine->m_scene;
}

Camera& EngineInternals::Camera()
{
    assert(pEngine != nullptr);
    return pEngine->m_camera;
}

