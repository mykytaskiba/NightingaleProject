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

    m_renderFunc = [](GameObject& gameObject) {
        gameObject.render();
    };

    m_syncPhysicsFunc = [](GameObject& gameObject) {
        gameObject.sync_to_physics();
    };


}
void Scene::tick()
{
    m_root.execute_on_hiearchy(m_tickFunc);
    m_root.execute_on_hiearchy(m_syncPhysicsFunc);

    for (auto it = m_packages.begin(); it != m_packages.end(); ++it) {
        (*it).processPackage();
    }
    m_packages.clear();
}

void Scene::render()
{
    m_root.execute_on_hiearchy(m_renderFunc);
}

void SceneChangePackage::processPackage()
{
    EngineFunctions::Setup(pGameObject);
}
