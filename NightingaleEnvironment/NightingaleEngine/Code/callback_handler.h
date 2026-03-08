#pragma once
#include "defines.h"
#include "priority_function.h"


struct CallbackRef {
	PriorityFunction* m_pFunction;
	set<PriorityFunction>::iterator it;
};

class CallbackHandler {

private:
	set<PriorityFunction> m_vFunctions;

public:
	CallbackRef addCallback(PriorityFunction function);
	bool removeCallback(CallbackRef& ref);
	void execute();
};