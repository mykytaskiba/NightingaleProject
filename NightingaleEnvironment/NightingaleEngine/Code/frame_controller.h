#pragma once
#include "defines.h"

//time_type defined in defines
class FrameController {

public:
    ~FrameController() {}

    FrameController(FrameController const&) = delete;
    FrameController& operator=(FrameController const&) = delete;

    void frameStart();
    void frameEnd();

    void setTargetFrameRate(uint target);

    FrameController(uint targetFrameRate);

    time_type getDeltaTime() { return m_deltaTime; }


private:

    time_type m_frameStartTime = 0;
    time_type m_frameEndTime = 0;
    time_type m_targetFrameRate = 0;

    time_type m_deltaTime = 0;

};