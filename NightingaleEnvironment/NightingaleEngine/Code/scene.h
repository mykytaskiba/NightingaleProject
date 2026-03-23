#pragma once
#include "game_object.h"
#include "defines.h"
#include "ngjson.h"

class Scene {

public:
    class SceneRootObject : public GameObject {
    public:
        SceneRootObject() : GameObject("scene_root") {}
    };

    constexpr static char c_JSONType[] = "json.scene";
    constexpr static uint c_JsonVersion = 1u;
    using JSONRepresentation = JSONRepresentation<Scene, c_JsonVersion, c_JSONType>;
    static bool upgradeJSON(JSONUpgrader& upgrader) { return false; }

    void properties(IPropertyVisitor& visitor) {

    }

private:
    using TDeferredFunction = std::function<void()>;

    std::vector<TDeferredFunction> m_defferedFunctions;
    void addDeferredFunction(TDeferredFunction function);

    //ptr for factory integration
    SceneRootObject* m_pRoot;


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
