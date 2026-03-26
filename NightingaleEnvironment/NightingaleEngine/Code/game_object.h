#pragma once
#include "core_interface.h"
#include "defines.h"
#include "ngmath.h"
#include "ngphys.h"
#include "guid.h"
#include "factory.h"
#include "json_object.h"
#include "transform.h"
#include "property_provider.h"
#include "property_visitor.h"

class Component;
class GameObject;
class RenderNode;
class IPropertyVisitor;

//typedef for different function types
//typedef void (*TGameObjectFunc)(GameObject&);
using TGameObjectFunc = std::function<void(GameObject&)>;
typedef void (*TComponentFunc)(Component&);

//Core gameobject class that keeps track of a parent-children hierarchy 
class GameObject : public FactoryElement<std::string, GameObject>, public IJSONObject, public IPropertyProvider {

    friend class EngineFunctions;
protected:


    std::string m_alias{ "gameobject" };
    GUID m_guid{};

    vector<Component*> m_components{};
    vector<GameObject*> m_vChildren{};
    GameObject* m_pParent{ nullptr };
    RenderNode* m_pRenderNode{ nullptr };
    PhysicsBody* m_pPhysicsBody{ nullptr };

    Transform m_transform{};

    Transform m_renderTransform{};

    virtual void shutdown() {}




public:

    JSON_PARENT(GameObject, 1u, "json.gameobject")

    void properties(IPropertyVisitor& visitor) override {
        visitor("alias", m_alias);
        visitor("transform", m_transform);

        if (m_pPhysicsBody != nullptr) {
            visitor("physics_body", *m_pPhysicsBody);
        }

        /*
        if (!m_vChildren.empty()) {
            visitor("child", m_vChildren[0]);
        }
        else {
            GameObject* pNullGameObject = (GameObject*) nullptr;
            visitor("child", pNullGameObject);
        }*/
    }


    GameObject();
    GameObject(std::string const& alias);

    Transform& getTransform() {
        return m_transform;
    }

    Transform const& getTransform() const {
        return m_transform;
    } 

    Transform const& getRenderTransform() const;

    std::string const& getAlias() const { return m_alias; }
    GUID const& getGUID() const { return m_guid; }

    void setAlias(std::string const& alias) { m_alias = alias; }

    void sync_physics_to_gameobject();
    void sync_gameobject_to_physics();


    virtual void init() {}
    virtual void tick() {}


    void execute_on_hierarchy(TGameObjectFunc functor);
    void execute_on_children(TGameObjectFunc functor);

    vector<GameObject*> const& getChildren() const { return m_vChildren; }
    vector<GameObject*>& getChildren() { return m_vChildren; }

    bool removeChild(GameObject*);

    void freeResources();

    GameObject* getParent() { return m_pParent; }

    
    bool hasRenderNode() { return m_pRenderNode != nullptr; }
    RenderNode const* getRenderNode() const { return m_pRenderNode; }
    RenderNode* getRenderNode() { return m_pRenderNode; }

    bool hasPhysicsBody() { return m_pPhysicsBody != nullptr; }
    PhysicsBody const* getPhysicsBody() const { return m_pPhysicsBody; }
    PhysicsBody* getPhysicsBody() { return m_pPhysicsBody; } 

};