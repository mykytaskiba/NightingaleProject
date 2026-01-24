#pragma once
#include "engine_system_binding.h"

class EngineSystem {
	virtual void register_system(EngineSystemBinding& binding) = 0;
};