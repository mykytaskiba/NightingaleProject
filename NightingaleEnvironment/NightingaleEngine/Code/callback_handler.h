#pragma once
#include "defines.h"
#include "priority_function.h"
#include <set>


struct CallbackRef {
	std::multiset<PriorityFunction>::iterator m_iterator;
};

class CallbackHandler {

private:
	std::multiset<PriorityFunction> m_vFunctions;
	
	bool m_bExecuting{ false };
	std::vector<CallbackRef> m_deferredRemovals; //deffer removals if called mid execution


public:
	CallbackRef addCallback(PriorityFunction function);
	bool removeCallback(CallbackRef ref);
	void execute();
};