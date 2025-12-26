#pragma once
#include "game_object.h"

class MeshGameObject : public GameObject {

protected:
    virtual void init();
    virtual void tick();
    virtual void render();
    virtual void shutdown();

    virtual RenderNode* createRenderNode();

};