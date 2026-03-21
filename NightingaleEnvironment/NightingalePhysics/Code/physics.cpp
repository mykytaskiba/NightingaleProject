#include "physics.h"
#include "nightingale_assert.h"
#include <iostream>

Physics::Physics()
{
	setTargetUpdateRate(DEFAULT_PHYSICS_FRAMERATE);
}

void Physics::update(TTimePhys frameDelta)
{

	if (!m_bActive) {
		return;
	}

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
			if (m_accumulatedTime > m_maxAccumulatedTime) {
				m_accumulatedTime = m_maxAccumulatedTime;
			}
			break;
		}
	}
}

void Physics::subUpdate(TTimePhys deltaT)
{

	//Gravity application step: TO DO: REFACTOR THIS!!!
	Vector3 gravity{ 0.0f,-9.8f,0.0f };
	for (PhysicsBody*& pBody : m_activeBodies) {
		if (!pBody->useGravity()) {
			continue;
		}
		pBody->setVelocity(gravity * deltaT + pBody->getVelocity());
	}


	//Collision check step
	for (PhysicsBody* pBody : m_activeBodies) {
		for (PhysicsBody* pOther : m_activeBodies) {
			if (pBody == pOther) continue;

			if (pBody->getGlobalBox().isOverlap(pOther->getGlobalBox())) {
				pBody->setVelocity(Vector3(0, 0, 0));
			}

		}
	}

	for (PhysicsBody*& pBody : m_activeBodies) {
		pBody->update(deltaT);
	}
}

void Physics::setTargetUpdateRate(uint updatesPerSecond)
{
	if (updatesPerSecond == 0) {
		assert(false);
		return;
	}

	m_infoUpdatesPerSecond = updatesPerSecond; 

	m_updateRate = (1.0f / (TTimePhys)updatesPerSecond);
	m_accumulatedTime = 0.0f;
}

void Physics::addAccumulatedTime(TTimePhys addedTime) {
	m_accumulatedTime += addedTime;
}


void Physics::addBody(PhysicsBody* pBody)
{
	m_activeBodies.push_back(pBody);
}

void Physics::removeBody(PhysicsBody* pBody)
{
	auto it = std::find(m_activeBodies.begin(), m_activeBodies.end(), pBody);
	if (it == m_activeBodies.end()) {
		assert(false);
		return;
	}
	m_activeBodies.erase(it);

}

