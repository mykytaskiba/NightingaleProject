#include "pch.h"
#include "engine_settings.h"
#include "engine.h"
#include "game_object.h"
#include "camera_controller.h"
#include "scene.h"

void Engine::defaultGameObjectFactory()
{
	m_factoryGameObject.addToFactory<GameObject>("gameobject");
	m_factoryGameObject.addToFactory<CameraController>("cameracontroller");
	m_factoryGameObject.addToFactory<Scene::SceneRootObject>("scene_root_object");

	m_factoryPhysicsBody.addToFactory<PhysicsBody>("physics_body");
}