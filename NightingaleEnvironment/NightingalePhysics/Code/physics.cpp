#include "physics.h"

Physics::Physics()
{
	SetTargetUpdateRate(DEFAULT_PHYSICS_FRAMERATE);
}


void Physics::update(TTimePhys frameDelta)
{
	m_accumulatedTime += frameDelta;

	uint updatesThisFrame{ 0u };

	while (m_accumulatedTime >= m_updateRate) {
		m_accumulatedTime += -m_updateRate;
		subUpdate(m_updateRate);

		++updatesThisFrame;
		if (updatesThisFrame >= m_maxUpdatesPerFrame) {
			if (m_bDiscardUnusedTime) {
				m_accumulatedTime = 0.0f;
			}
			break;
		}
	}
}

void Physics::subUpdate(TTimePhys deltaT)
{
}

void Physics::SetTargetUpdateRate(uint updatesPerSecond)
{
	m_updateRate = (1.0f / (TTimePhys)updatesPerSecond);
	m_accumulatedTime = 0.0f;
}

void Physics::SetMaxUpdatesPerFrame(uint maxUpdatePerFrame)
{
	m_maxUpdatesPerFrame = maxUpdatePerFrame;
}

void Physics::SetDiscardUnusedTime(bool bDiscardUnusedTime)
{
	m_bDiscardUnusedTime = bDiscardUnusedTime;
}

