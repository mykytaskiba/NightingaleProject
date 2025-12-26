#pragma once
#include "frame_controller.h"

//Static only class for time related data
class Time {

    friend class Engine;

public:
    static time_type GetDelta() { return s_delta; }
private:
    static time_type s_delta;

};