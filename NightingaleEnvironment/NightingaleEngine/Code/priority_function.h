#pragma once
#include "defines.h"

struct PriorityFunction {
	
	uint priority; 
	FEngineProcedure function;

	PriorityFunction(uint inPriority, FEngineProcedure inFunction) : priority(inPriority), function(inFunction) {}

	void operator()() const {
		function();
	}

	bool operator<(PriorityFunction const& other) const {
		return priority < other.priority;
	}
	
};