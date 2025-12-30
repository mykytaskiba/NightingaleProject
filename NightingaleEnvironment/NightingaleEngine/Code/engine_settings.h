#pragma once
#include "defines.h"
#include "afterparty.h"

struct EngineSettings {

	string window_title{};

	vector<AfterpartyCommand*> console_commands{};

	vector<string> load_commands{};
};