#include "pch.h"
#include "scene.h"
#include "game_object.h"
#include "engine_functions.h"
#include "camera_controller.h"
#include "loader.h"
#include "render_mesh.h"

void Scene::addDeferredFunction(TDeferredFunction function)
{
    m_defferedFunctions.push_back(function);
}

void Scene::init()
{
    m_tickFunc = [](GameObject& gameObject) {
        gameObject.tick();
    };

    GameObject* pGameObjectRoot{ nullptr };
    EngineFunctions::factoryGameObject().create("scene_root_object", pGameObjectRoot);

    if (pGameObjectRoot == nullptr) {
        assert(false);
        return;
    }

    m_pRoot = dynamic_cast<SceneRootObject*>(pGameObjectRoot);
    if (m_pRoot == nullptr) {
        assert(false);
        return;
    }


}
void Scene::tick()
{
    if (m_pRoot == nullptr) {
        assert(false);
        return;
    }
    m_pRoot->execute_on_hierarchy(m_tickFunc);

    for (TDeferredFunction const& function : m_defferedFunctions) {
        function();
    }
    m_defferedFunctions.clear();
}

void Scene::sync_gameobjects_to_physics()
{
    execute_on_root(
        [](GameObject& gameObject) {
            gameObject.sync_gameobject_to_physics();
        }
    );
}

void Scene::sync_physics_to_gameobjects()
{
    execute_on_root(
        [](GameObject& gameObject) {
            gameObject.sync_physics_to_gameobject();
        }
    );
}


void Scene::execute_on_root(TGameObjectFunc func)
{

    if (m_pRoot == nullptr) {
        assert(false);
        return;
    }
    m_pRoot->execute_on_hierarchy(func);
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


void Scene::delete_object(GameObject* pGameObject)
{
    if (pGameObject == nullptr) {
        assert(false); //why are we deleting something that already null? 
        return;
    }

    GameObject* pParent = pGameObject->getParent();
    if (pParent == nullptr) {
        assert(false); //we should never delete anything that doesnt have a parent. Including the scene root
        return;
    }

    deleteChildren(pGameObject);

    pParent->removeChild(pGameObject);

    pGameObject->freeResources();
    
    delete pGameObject;
    pGameObject = nullptr;
}

void Scene::deleteChildren(GameObject* pGameObject)
{
    while (!pGameObject->getChildren().empty()) {
        auto it = pGameObject->getChildren().end() - 1u;

        GameObject* pChild = *it;
        pGameObject->getChildren().erase(it);
        if (pChild == nullptr) {
            assert(false);
            return;
        }

        delete_object(pChild);
    }

    assert(pGameObject->getChildren().empty());
}

void Scene::clearScene()
{
    deleteChildren(get_root());
}
