#include "pch.h"
#include "gameobject_factory.h"
#include "engine_functions.h"

//Game Object includes
#include "game_object.h";
#include "camera_controller.h"
#include "project_one_gameobject.h"

//TO DO REFACTOR THIS, GET PPARENT OUT OF HERE!!!
bool GameObjectFactory::instantiateFromType(string const& type, GameObject*& pResult, GameObject* pParent)
{
    pResult = nullptr;

    if (type == "gameobject") {
        pResult = EngineFunctions::InstantiateGameObject<GameObject>(pParent);
    }
    if (type == "cameracontroller") {
        pResult = EngineFunctions::InstantiateGameObject<CameraController>(pParent);
    }
    if (type == "project1gameobject") {
        pResult = EngineFunctions::InstantiateGameObject<ProjectOneGameObject>(pParent);
    }

    return pResult != nullptr;
}
