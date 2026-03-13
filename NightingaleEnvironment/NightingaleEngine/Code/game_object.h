#pragma once
#include "core_interface.h"
#include "defines.h"
#include "ngmath.h"
#include "ngphys.h"

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

    GameObject() {};
     
    vector<Component*> m_components{};
    vector<GameObject*> m_children{};
    GameObject* m_pParent{ nullptr };
    RenderNode* m_pRenderNode{ nullptr };

    Transform m_transform{};

    virtual void init() {}
    virtual void tick() {}
    virtual void render() {}
    virtual void shutdown() {}


    void execute_on_hierarchy(TGameObjectFunc functor);
    

public:
    Transform& getTransform() {
        return m_transform;
    }

    void sync_to_physics();

    PhysicsBody* m_pPhysicsBody{ nullptr };

};