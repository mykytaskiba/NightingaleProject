#pragma once
#include "defines.h"
#include "priority_function.h"


struct CallbackRef {
	set<PriorityFunction>::iterator m_iterator;
};

class CallbackHandler {

private:
	set<PriorityFunction> m_vFunctions;

public:
	CallbackRef addCallback(PriorityFunction function);
	bool removeCallback(CallbackRef ref);
	void execute() const;
};