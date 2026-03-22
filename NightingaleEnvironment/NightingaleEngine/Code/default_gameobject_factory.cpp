#include "pch.h"
#include "engine_settings.h"
#include "engine.h"
#include "game_object.h"
#include "camera_controller.h"

void Engine::defaultGameObjectFactory()
{
	m_factoryGameObject.addToFactory<GameObject>("gameobject");
	m_factoryGameObject.addToFactory<CameraController>("cameracontroller");
}