#pragma once
#include "defines.h"

class GameObject;

//simple class to create all types of gameobjects
class GameObjectFactory {
public:
    static bool instantiateFromType(string const& type, GameObject*& result);
};