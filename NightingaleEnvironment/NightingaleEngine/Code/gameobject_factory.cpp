#include "pch.h"
#include "gameobject_factory.h"
#include "engine_functions.h"

//Game Object includes
#include "game_object.h";
#include "camera_controller.h"
#include "project_one_gameobject.h"

bool GameObjectFactory::instantiateFromType(string const& type, GameObject*& pResult)
{
    pResult = nullptr;

    if (type == "gameobject") {
        pResult = EngineFunctions::InstantiateGameObject<GameObject>();
    }
    if (type == "cameracontroller") {
        pResult = EngineFunctions::InstantiateGameObject<CameraController>();
    }
    if (type == "project1gameobject") {
        pResult = EngineFunctions::InstantiateGameObject<ProjectOneGameObject>();
    }

    return pResult != nullptr;
}
