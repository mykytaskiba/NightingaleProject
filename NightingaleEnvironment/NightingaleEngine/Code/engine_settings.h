#pragma once
#include "defines.h"
#include "scripting.h"
#include "factory.h"

class GameObject;

struct EngineSettings {
	vector<CommandInterface*> console_commands{};
	vector<string> load_commands{};

	std::function<void(Factory<std::string, GameObject>&)> m_setupFactoryCallback{ nullptr };
};