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
protected:

    GameObject() {};
    GameObject(std::string const& alias) : m_alias(alias) {};

    std::string m_alias{ "gameobject" };
     
    vector<Component*> m_components{};
    vector<GameObject*> m_children{};
    GameObject* m_pParent{ nullptr };
    RenderNode* m_pRenderNode{ nullptr };

    Transform m_transform{};

    virtual void shutdown() {}


    

public:
    Transform& getTransform() {
        return m_transform;
    }

    std::string const& getAlias() const { return m_alias; }
    void setAlias(std::string const& alias) { m_alias = alias; }

    void sync_to_physics();
    virtual void init() {}
    virtual void tick() {}

    PhysicsBody* m_pPhysicsBody{ nullptr };

    void execute_on_hierarchy(TGameObjectFunc functor);
    void execute_on_children(TGameObjectFunc functor);

    vector<GameObject*> const& get_children() const { return m_children; }
    vector<GameObject*>& get_children() { return m_children; }

};