#include "pch.h"
#include "termination.h"

bool Termination::s_bTerminationSignal = false;

void Termination::SendTerminationSignal() {
    s_bTerminationSignal = true;
}

bool Termination::NeedTermination()
{
    return s_bTerminationSignal;
}
