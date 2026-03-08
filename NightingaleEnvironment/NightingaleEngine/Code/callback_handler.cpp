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

    m_vFunctions.erase(ref.m_iterator);
    return true;
}

void CallbackHandler::execute() const
{
    for (const PriorityFunction& pFunction : m_vFunctions) {
        (pFunction.function)();
    }
}
