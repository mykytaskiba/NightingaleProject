#include "pch.h"
#include "scene.h"
#include "game_object.h"
#include "engine_functions.h"
#include "camera_controller.h"
#include "loader.h"
#include "render_mesh.h"

void Scene::addPackage(SceneChangePackage package)
{
    m_packages.push_back(package);
}

void Scene::init()
{
    m_tickFunc = [](GameObject& gameObject) {
        gameObject.tick();
    };


    m_root.setAlias("scene_root");


}
void Scene::tick()
{
    m_root.execute_on_hierarchy(m_tickFunc);

    for (auto it = m_packages.begin(); it != m_packages.end(); ++it) {
        (*it).processPackage();
    }
    m_packages.clear();
}

void Scene::sync_gameobjects_to_physics()
{
    m_root.execute_on_hierarchy(
        [](GameObject& gameObject) {
            gameObject.sync_gameobject_to_physics();
        }
    );
}

void Scene::sync_physics_to_gameobjects()
{
    m_root.execute_on_hierarchy(
        [](GameObject& gameObject) {
            gameObject.sync_physics_to_gameobject();
        }
    );
}


void Scene::execute_on_root(TGameObjectFunc func)
{
    m_root.execute_on_hierarchy(func);
}

GameObject* Scene::find_object(GUID const& guid)
{
    GameObject* pResult = nullptr;

    execute_on_root(
        [this, &guid, &pResult](GameObject& gameObject) {
            if (gameObject.getGUID() == guid) {
                pResult = &gameObject;
            }
        }
    );

    return pResult;
}

void SceneChangePackage::processPackage()
{
    EngineFunctions::Setup(pGameObject);
}
