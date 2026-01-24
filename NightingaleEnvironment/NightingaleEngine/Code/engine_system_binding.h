#pragma once
#include "defines.h"

class Engine;
class EngineSystemBinding {
	Engine& m_engine;
public:
	EngineSystemBinding(Engine& engine) : m_engine(engine) {}

	void register_init(uint priority, FEngineProcedure function);
	void register_update(uint priority, FEngineProcedure function);
	void register_shutdown(uint priority, FEngineProcedure function);
};
