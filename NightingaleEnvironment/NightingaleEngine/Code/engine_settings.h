#pragma once
#include "defines.h"
#include "scripting.h"
#include "game_object.h"
#include "factory.h"

class GameObject;

struct EngineSettings {
	vector<CommandInterface*> console_commands{};
	vector<string> load_commands{};

};