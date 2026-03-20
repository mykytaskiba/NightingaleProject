#include "pch.h"
#include "special_arguments.h"
#include "engine_functions.h"
#include "execution_state.h"
#include "scene.h"
#include "argument_impl.h"

bool SelectedGameObject::retrieve(ParsingResult& result)
{
    GUID guid = EngineFunctions::execution_state().getSelectedGUID();
    if (guid.isInvalid()) {
        result.bSuccess = false;
        result.errorMessage = "Invalid GUID selected " + guid.string();
        return false;
    }

    m_pGameObject = EngineFunctions::scene().find_object(guid);
    if (m_pGameObject == nullptr) {
        result.bSuccess = false;
        result.errorMessage = "Selected Object not found by GUID " + guid.string();
        return false;
    }

    result.bSuccess = true;
    return true;
}