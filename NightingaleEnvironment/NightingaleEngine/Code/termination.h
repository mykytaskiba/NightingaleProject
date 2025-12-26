#pragma once


//Termination mechanism for the engine
class Termination {

    static bool s_bTerminationSignal;
public:
    static void SendTerminationSignal();
    static bool NeedTermination();
};