#include "pch.h"
#include "physics_commands.h"
#include "engine_functions.h"
#include "ngphys.h"
#include "game_object.h"

void PhysicsActiveCommand::execute_command(ArgumentList<TriBool>& args, ExecutionState& state, ExecutionResult& result)
{
	bool bValue = EngineFunctions::physics().isActive();
	s_evaluateTriBool(bValue, args.get<0>());
	EngineFunctions::physics().setActive(bValue);
}

void AttachPhysicsBodyCommand::execute_command(ArgumentList<Nothing>& args, ExecutionState& state, ExecutionResult& result)
{

	GameObject* pGameObject{ nullptr };
	if (!state.getSelectedObject(pGameObject, result)) {
		return;
	}

	if (pGameObject->hasPhysicsBody()) {
		result.bSuccess = false;
		result.message = "Gameobject already has a physics body";
		return;
	}

	EngineFunctions::AttachPhysicsBody(pGameObject);
	result.bSuccess = true;
	result.message = "Physics Body added to object " + pGameObject->getAlias();

}
