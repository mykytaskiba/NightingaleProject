#pragma once
#include "game_object.h"
#include "defines.h"

struct SceneChangePackage;

class Scene {
    friend class EngineFunctions;
private:

    class SceneRootObject : public GameObject {
    public:
        SceneRootObject() : GameObject("scene_root") {}
    };

    SceneRootObject m_root;

    vector<SceneChangePackage> m_packages;
    void addPackage(SceneChangePackage package);

public:

    TGameObjectFunc m_tickFunc;
    TGameObjectFunc m_syncPhysicsFunc;

    void init();
    void tick();
    
    void execute_on_root(TGameObjectFunc func);

    GameObject* get_root() {
        return &m_root;
    }

};


//when a something in the scene changes, this is packages the change
struct SceneChangePackage {
    GameObject* pGameObject;

    void processPackage();
};