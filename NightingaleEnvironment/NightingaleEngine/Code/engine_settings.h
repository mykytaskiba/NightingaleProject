#pragma once
#include "defines.h"
#include "scripting.h"
#include "game_object.h"
#include "factory.h"

class GameObject;

struct EngineSettings {
	std::vector<CommandInterface*> console_commands{};
	std::vector<string> load_commands{};

};