#pragma once
#include "game_object.h"
#include "defines.h"

struct SceneChangePackage;

class Scene {
    friend class EngineFunctions;
private:
    GameObject m_root;

    vector<SceneChangePackage> m_packages;
    void addPackage(SceneChangePackage package);

public:

    TGameObjectFunc m_tickFunc;
    TGameObjectFunc m_renderFunc;
    TGameObjectFunc m_syncPhysicsFunc;

    void init();

    void tick();
    void render();

};

//when a something in the scene changes, this is packages the change
struct SceneChangePackage {
    GameObject* pGameObject;

    void processPackage();
};