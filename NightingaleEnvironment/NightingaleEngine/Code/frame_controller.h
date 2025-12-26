#pragma once
#include "defines.h"

using time_type = double;

class FrameController {

public:
    FrameController(uint targetFrameRate) {
        setTargetFrameRate(targetFrameRate);
    }
    ~FrameController() {}

    FrameController(FrameController const&) = delete;
    FrameController& operator=(FrameController const&) = delete;

    void frameStart();
    void frameEnd();

    void setTargetFrameRate(uint target);

    time_type getDeltaTime() { return m_deltaTime; }


private:

    time_type m_frameStartTime = 0;
    time_type m_frameEndTime = 0;
    time_type m_targetFrameRate = 0;

    time_type m_deltaTime = 0;

};