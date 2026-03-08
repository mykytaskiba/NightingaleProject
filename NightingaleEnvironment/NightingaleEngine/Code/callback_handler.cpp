#include "pch.h"
#include "callback_handler.h"

CallbackRef CallbackHandler::addCallback(PriorityFunction function)
{
    PriorityFunction* pFunction = new PriorityFunction(function);
    m_vFunctions.insert(pFunction);

    CallbackRef ref = { pFunction };
    return ref;
}

bool CallbackHandler::removeCallback(CallbackRef& ref)
{
    if (ref.m_pFunction == nullptr) return false;

    auto it = m_vFunctions.find(ref.m_pFunction);
    if (it == m_vFunctions.end()) {
        return false;
    }

    m_vFunctions.erase(it);

    delete ref.m_pFunction;
    ref.m_pFunction = nullptr;

    return true;
}

void CallbackHandler::execute()
{
    for (PriorityFunction* pFunction : m_vFunctions) {
        (pFunction->function)();
    }
}
