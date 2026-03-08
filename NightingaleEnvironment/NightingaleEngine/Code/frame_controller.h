#pragma once
#include "defines.h"

#define DEFAULT_FRAMERATE_TARGET 10 

//time_type defined in defines
class FrameController {

public:

    FrameController();

    void frameStart();
    void frameEnd();

    void setTargetFrameRate(uint target);
    float getTargetFrameRate();


    TTimeDelta getDeltaTime() { return m_deltaTime; }


private:

    TTimeAbs m_frameStartTime{ 0.0 };
    TTimeAbs m_frameEndTime{ 0.0 };
    TTimeAbs m_targetDeltaTime{ 0.0 };

    TTimeDelta m_deltaTime{ 0.0 };

};