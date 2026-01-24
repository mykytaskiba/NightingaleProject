#pragma once
#include "defines.h"
#include "scripting.h"

struct EngineSettings {
	vector<CommandInterface*> console_commands{};
	vector<string> load_commands{};
};