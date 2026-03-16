#include "pch.h"
#include "physics_commands.h"
#include "engine_functions.h"
#include "ngphys.h"

void PhysicsActiveCommand::execute_command(ArgumentList<TriBool>& args, ExecutionState& state, ExecutionResult& result)
{
	bool bValue = EngineFunctions::physics().isActive();
	s_evaluateTriBool(bValue, args.get<0>());
	EngineFunctions::physics().setActive(bValue);
}
