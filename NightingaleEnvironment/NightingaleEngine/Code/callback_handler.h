#pragma once
#include "defines.h"
#include "priority_function.h"


struct CallbackRef {
	set<PriorityFunction>::iterator m_iterator;
};

class CallbackHandler {

private:
	set<PriorityFunction> m_vFunctions;
	
	bool m_bExecuting{ false };
	vector<CallbackRef> m_deferredAdds; //deffer additions if called mid execution
	vector<CallbackRef> m_deferredRemovals; //deffer removals if called mid execution


public:
	CallbackRef addCallback(PriorityFunction function);
	bool removeCallback(CallbackRef ref);
	void execute();
};