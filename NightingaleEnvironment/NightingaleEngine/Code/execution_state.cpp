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


