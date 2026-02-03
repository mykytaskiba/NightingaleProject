#include "physics.h"
#include "nightingale_assert.h"

Physics::Physics()
{
	setTargetUpdateRate(DEFAULT_PHYSICS_FRAMERATE);
}

#include <iostream>
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

	//Gravity application step: TO DO: REFACTOR THIS!!!
	Vector3 gravity{ 0.0f,-9.8f,0.0f };
	for (Body*& pBody : m_activeBodies) {
		if (!pBody->bUseGravity) {
			continue;
		}
		pBody->velocity += gravity * deltaT;
	}


	//Collision check step
	for (Body* pBody : m_activeBodies) {
		for (Body* pOther : m_activeBodies) {
			if (pBody == pOther) continue;

			if (pBody->globalBox.isOverlap(pOther->globalBox)) {
				pBody->velocity = Vector3(0, 0, 0);
			}

		}
	}

	for (Body*& pBody : m_activeBodies) {
		pBody->update(deltaT);
	}
}

void Physics::setTargetUpdateRate(uint updatesPerSecond)
{
	if (updatesPerSecond == 0) {
		assert(false);
		return;
	}
	m_updateRate = (1.0f / (TTimePhys)updatesPerSecond);
	m_accumulatedTime = 0.0f;
}

void Physics::setMaxUpdatesPerFrame(uint maxUpdatePerFrame)
{
	m_maxUpdatesPerFrame = maxUpdatePerFrame;
}

void Physics::setDiscardUnusedTime(bool bDiscardUnusedTime)
{
	m_bDiscardUnusedTime = bDiscardUnusedTime;
}

void Physics::addBody(Body* pBody)
{
	m_activeBodies.push_back(pBody);
}

