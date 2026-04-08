#pragma once
#include "transform.h"

class GraphicsContext;

class RenderNode {
    //A render node is paired with a gameobject. It is responsible for rendering that specific object
protected:
    Transform m_transform;
public:
    void setNodePosition(Transform const& transform) { m_transform = transform; };
    virtual void render(GraphicsContext& context) {}
};