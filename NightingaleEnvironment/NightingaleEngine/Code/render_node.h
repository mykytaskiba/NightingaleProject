#pragma once

class GameObject;
class GraphicsContext;

class RenderNode {
    //A render node is paired with a gameobject. It is responsible for rendering that specific object
public:
    virtual void init(GameObject* pParent) {}
    virtual void render(GraphicsContext& context) {}
};