#pragma once
#include "game_object.h"
#include "defines.h"
#include "json_object.h"

class Scene : public IJSONObject {

public:
    class SceneRootObject : public GameObject {
    public:
        SceneRootObject() : GameObject("scene_root") {}

        JSON_CHILD(SceneRootObject, 2u, GameObject)
    };

    JSON_PARENT(Scene, 1u, "json.scene")


    int testInt;
    std::vector<int> vectorInt;

    void properties(IPropertyVisitor& visitor) {
        visitor("root", m_pRoot);
        visitor("test int", testInt);
    }

private:
    using TDeferredFunction = std::function<void()>;

    std::vector<TDeferredFunction> m_defferedFunctions;
    void addDeferredFunction(TDeferredFunction function);

    //ptr for factory integration
    GameObject* m_pRoot;


public:

    TGameObjectFunc m_tickFunc;

    void init();
    void tick();
    void syncObjectToPhysics();
    void syncPhysicsToObject();
    
    void executeOnRoot(TGameObjectFunc func);

    GameObject* findObject(GUID const& guid);

    GameObject* getRoot() {
        return m_pRoot;
    }
    void deleteObject(GameObject* pGameObject);
    void deleteChildren(GameObject* pGameObject);
    void addObject(GameObject* pGameObject, GameObject* pParent);

    void clearScene();
};
