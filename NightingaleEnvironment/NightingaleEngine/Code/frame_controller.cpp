#include "pch.h"
#include "frame_controller.h"
#include <GLFW/glfw3.h>

void FrameController::frameStart()
{
	m_frameStartTime = glfwGetTime();
}

void FrameController::frameEnd()
{
	m_frameEndTime = glfwGetTime();

	while ( (m_deltaTime = m_frameEndTime - m_frameStartTime)  < m_targetFrameRate) {
		m_frameEndTime = glfwGetTime();
	}

	m_deltaTime = m_frameEndTime - m_frameStartTime;

}

void FrameController::setTargetFrameRate(uint target)
{ 
	assert(target != 0);
	m_targetFrameRate = 1.0 / ((time_type)target);
}
