#include "pch.h"
#include "engine_system_binding.h"
#include "engine.h"

void EngineSystemBinding::register_init(uint priority, FEngineProcedure function)
{
	PriorityFunction func(priority, function);
	
	m_engine.m_initFunctions.insert(func);
}

void EngineSystemBinding::register_update(uint priority, FEngineProcedure function)
{
	PriorityFunction func(priority, function);

	m_engine.m_updateFunctions.insert(func);
}

void EngineSystemBinding::register_shutdown(uint priority, FEngineProcedure function)
{
	PriorityFunction func(priority, function);

	m_engine.m_shutdownFunctions.insert(func);
}
