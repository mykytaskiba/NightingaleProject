#include "pch.h"
#include "callback_handler.h"

CallbackRef CallbackHandler::addCallback(PriorityFunction function)
{
    auto pairReturn = m_vFunctions.insert(function);

    if (pairReturn.second == true) {
        CallbackRef ref{ pairReturn.first }; 
        return ref;
    }

    return CallbackRef{};
}

bool CallbackHandler::removeCallback(CallbackRef ref)
{
    if (m_bExecuting) {
        m_deferredRemovals.push_back(ref);
        return true;
    }

    m_vFunctions.erase(ref.m_iterator);
    return true;
}

void CallbackHandler::execute()
{
    m_bExecuting = true;
    for (const PriorityFunction& pFunction : m_vFunctions) {
        (pFunction.function)();
    }
    m_bExecuting = false;

    for (CallbackRef const& ref : m_deferredRemovals) {
        removeCallback(ref);
    }
    m_deferredRemovals.clear();


}
