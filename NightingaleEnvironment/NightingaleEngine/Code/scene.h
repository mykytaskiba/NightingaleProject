#pragma once
#include "game_object.h"
#include "defines.h"

class Scene {
    friend class EngineFunctions;

public:
    class SceneRootObject : public GameObject {
    public:
        SceneRootObject() : GameObject("scene_root") {}
    };
private:


    using TDeferredFunction = std::function<void()>;

    //ptr for factory integration
    SceneRootObject* m_pRoot;

    std::vector<TDeferredFunction> m_defferedFunctions;
    void addDeferredFunction(TDeferredFunction function);

public:

    TGameObjectFunc m_tickFunc;

    void init();
    void tick();
    void sync_gameobjects_to_physics();
    void sync_physics_to_gameobjects();
    
    void execute_on_root(TGameObjectFunc func);

    GameObject* find_object(GUID const& guid);

    GameObject* get_root() {
        return m_pRoot;
    }
    void delete_object(GameObject* pGameObject);
    void deleteChildren(GameObject* pGameObject);

    void clearScene();
};
