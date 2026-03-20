#pragma once
#include "game_object.h"
#include "defines.h"

class Scene {
    friend class EngineFunctions;
private:

    class SceneRootObject : public GameObject {
    public:
        SceneRootObject() : GameObject("scene_root") {}
    };

    using TDeferredFunction = std::function<void()>;

    SceneRootObject m_root;

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
        return &m_root;
    }
    void delete_object(GameObject* pGameObject);
    void deleteChildren(GameObject* pGameObject);

    void clearScene();
};
