#pragma once
#include "core_interface.h"
#include "defines.h"
#include "ngmath.h"
#include "ngphys.h"
#include "guid.h"

class Component;
class GameObject;
class RenderNode;

//typedef for different function types
//typedef void (*TGameObjectFunc)(GameObject&);
using TGameObjectFunc = std::function<void(GameObject&)>;
typedef void (*TComponentFunc)(Component&);

//Core gameobject class that keeps track of a parent-children hierarchy 
class GameObject {

    friend class EngineFunctions;
protected:

    GameObject();
    GameObject(std::string const& alias);

    std::string m_alias{ "gameobject" };
    GUID m_guid{};
     
    vector<Component*> m_components{};
    vector<GameObject*> m_children{};
    GameObject* m_pParent{ nullptr };
    RenderNode* m_pRenderNode{ nullptr };
    PhysicsBody* m_pPhysicsBody{ nullptr };

    Transform m_transform{};

    virtual void shutdown() {}


    

public:
    Transform& getTransform() {
        return m_transform;
    }

    std::string const& getAlias() const { return m_alias; }
    GUID const& getGUID() const { return m_guid; }

    void setAlias(std::string const& alias) { m_alias = alias; }

    void sync_physics_to_gameobject();
    void sync_gameobject_to_physics();


    virtual void init() {}
    virtual void tick() {}


    void execute_on_hierarchy(TGameObjectFunc functor);
    void execute_on_children(TGameObjectFunc functor);

    vector<GameObject*> const& get_children() const { return m_children; }
    vector<GameObject*>& get_children() { return m_children; }

    
    bool hasRenderNode() { return m_pRenderNode != nullptr; }
    RenderNode const* getRenderNode() const { return m_pRenderNode; }
    RenderNode* getRenderNode() { return m_pRenderNode; }

    bool hasPhysicsBody() { return m_pPhysicsBody != nullptr; }
    PhysicsBody const* getPhysicsBody() const { return m_pPhysicsBody; }
    PhysicsBody* getPhysicsBody() { return m_pPhysicsBody; } 

};