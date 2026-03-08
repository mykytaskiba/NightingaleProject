#include "pch.h"
#include "frame_controller.h"
#include <GLFW/glfw3.h>

FrameController::FrameController()
{
	setTargetFrameRate(DEFAULT_FRAMERATE_TARGET);
}

void FrameController::frameStart()
{
	m_frameStartTime = glfwGetTime();
}

void FrameController::frameEnd()
{
	m_frameEndTime = glfwGetTime();

	while ( (m_deltaTime = m_frameEndTime - m_frameStartTime)  < m_targetDeltaTime) {
		m_frameEndTime = glfwGetTime();
	}

	m_deltaTime = m_frameEndTime - m_frameStartTime;

}

void FrameController::setTargetFrameRate(uint target)
{ 
	assert(target != 0);
	if (target == 0) {
		target = DEFAULT_FRAMERATE_TARGET; //prevent crash, assume default framerate target 
	}
	m_targetDeltaTime = 1.0 / ((TTimeAbs)target);
}

float FrameController::getTargetFrameRate()
{
	return 1.0f / m_targetDeltaTime;
}
