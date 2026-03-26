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
	for (PhysicsBody*& pBody : m_vActiveBodies) {
		if (!pBody->useGravity()) {
			continue;
		}
		pBody->setVelocity(gravity * deltaT + pBody->getVelocity());
	}

	m_infoSpatialPairsCount = 0;

	//Collision check step
	m_spatialStructure.queryPairs(
		[this](PhysicsBody* pBody, PhysicsBody* pOther) {
			++m_infoSpatialPairsCount;
			if (pBody->getGlobalBox().isOverlap(pOther->getGlobalBox())) {
				pBody->setVelocity(Vector3(0, 0, 0));
				pOther->setVelocity(Vector3(0, 0, 0));
			}
		}
	);

	for (PhysicsBody*& pBody : m_vActiveBodies) {
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
	m_vActiveBodies.push_back(pBody);
	m_spatialStructure.insert(pBody);
}

void Physics::removeBody(PhysicsBody* pBody)
{
	std::erase(m_vActiveBodies, pBody);
	m_spatialStructure.remove(pBody);

}

