#pragma once
#include "game_object.h"
#include "defines.h"
#include "json_object.h"

class Scene : public IJSONObject, public IPropertyProvider {

public:
    class SceneRootObject : public GameObject {
    public:
        SceneRootObject() : GameObject("scene_root") {}

        JSON_CHILD(SceneRootObject, 1u, GameObject)

        void properties(IPropertyVisitor& visitor) override {
            std::string factoryKeyStr{ getFactoryKey() };
            visitor("factory_key", factoryKeyStr, MetaData::ReadOnly());
            visitor("alias", m_alias);
        }
    };

    JSON_PARENT(Scene, 1u, "json.scene")


    int testInt;
    std::vector<int> vectorInt{ 1,2,3,4,5,6 };

    std::vector<GameObject*> m_vSceneObjects{};

    void properties(IPropertyVisitor& visitor) override {
        visitor("root", m_pRoot);
        visitor("test int", testInt);
        visitor("vector int", vectorInt);

        test_collect_objects();
        visitor("gameobjects", m_vSceneObjects);
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

    void test_collect_objects();
    
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
