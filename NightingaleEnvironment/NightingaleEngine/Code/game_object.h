#pragma once
#include "core_interface.h"
#include "defines.h"
#include "vector.h"
#include "transform.h"

class Component;
class GameObject;
class RenderNode;

//typedef for different function types
typedef void (*TGameObjectFunc)(GameObject&);
typedef void (*TComponentFunc)(Component&);

//Core gameobject class that keeps track of a parent-children hierarchy 
class GameObject {

    friend class EngineFunctions;
    friend class Scene;
protected:

    GameObject();

    vector<Component*> m_components;
    vector<GameObject*> m_children;
    GameObject* m_pParent;
    RenderNode* m_pRenderNode;


    virtual void init() {}
    virtual void tick() {}
    virtual void render() {}
    virtual void shutdown() {}

    virtual RenderNode* createRenderNode() { return nullptr; }

    void execute(TGameObjectFunc functor);

    Transform m_transform{};

public:
    Transform& getTransform() {
        return m_transform;
    }




};