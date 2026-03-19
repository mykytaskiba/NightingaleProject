#include "pch.h"
#include "execution_state.h"
#include "engine_functions.h"
#include "scene.h"

GUID ExecutionState::getSelectedGUID() const
{
    int guid_as_int = extract_or_default<int>("selected_guid", -1);
    
    if (guid_as_int < 0) {
        return GUID{};
    }
    return GUID::From(guid_as_int);
}

void ExecutionState::setSelectedGUID(GUID const& guid)
{
    defines["selected_guid"] = guid.string();
}

bool ExecutionState::getSelectedObject(GameObject*& pGameObject, ExecutionResult& result) const
{
    GUID guid = getSelectedGUID();
    if (guid.isInvalid()) {
        result.bSuccess = false;
        result.message = "Invalid GUID selected " + guid.string();
        return false;
    }

    pGameObject = EngineFunctions::scene().find_object(guid);

    if (pGameObject == nullptr) {
        result.bSuccess = false;
        result.message = "Selected Object not found by GUID " + guid.string();
        return false;
    }

    return true;
}

