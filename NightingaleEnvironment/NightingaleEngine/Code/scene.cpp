#include "pch.h"
#include "scene.h"
#include "game_object.h"
#include "engine_functions.h"
#include "camera_controller.h"
#include "loader.h"
#include "render_mesh.h"
#include "ngjson.h"


JSON_IMPL(Scene)
JSON_IMPL(Scene::SceneRootObject)

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

void Scene::syncObjectToPhysics()
{
    executeOnRoot(
        [](GameObject& gameObject) {
            gameObject.sync_gameobject_to_physics();
        }
    );
}

void Scene::syncPhysicsToObject()
{
    executeOnRoot(
        [](GameObject& gameObject) {
            gameObject.sync_physics_to_gameobject();
        }
    );
}

void Scene::test_collect_objects()
{
    m_vSceneObjects.clear();
    executeOnRoot(
        [this](GameObject& gameObject) {
            m_vSceneObjects.push_back(&gameObject);
        }
    );
}


void Scene::executeOnRoot(TGameObjectFunc func)
{

    if (m_pRoot == nullptr) {
        assert(false);
        return;
    }
    m_pRoot->execute_on_hierarchy(func);
}

GameObject* Scene::findObject(GUID const& guid)
{
    GameObject* pResult = nullptr;

    executeOnRoot(
        [this, &guid, &pResult](GameObject& gameObject) {
            if (gameObject.getGUID() == guid) {
                pResult = &gameObject;
            }
        }
    );

    return pResult;
}


void Scene::deleteObject(GameObject* pGameObject)
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

        deleteObject(pChild);
    }

    assert(pGameObject->getChildren().empty());
}

void Scene::addObject(GameObject* pGameObject, GameObject* pParent)
{
    addDeferredFunction([pGameObject, pParent] {
        EngineFunctions::Setup(pGameObject, pParent);
        }
    );
}

void Scene::clearScene()
{
    deleteChildren(getRoot());
}
