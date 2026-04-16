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
		[this](PhysicsBody& body, PhysicsBody& other) {
			resolvePossibleCollision(body, other);
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

void Physics::resolvePossibleCollision(PhysicsBody& body, PhysicsBody& other)
{
	++m_infoSpatialPairsCount;

	if (!body.hasShape() || !other.hasShape()) {
		//would this ever be the case?
		return;
	}

	if (!broadPhase(body, other)) {
		return;
	}

	if (!narrowPhase(body, other)) {
		return;
	}

	body.setVelocity(Vector3(0, 0, 0));
	other.setVelocity(Vector3(0, 0, 0));
}

bool Physics::broadPhase(PhysicsBody& body, PhysicsBody& other)
{
	return body.getGlobalBox().isOverlap(other.getGlobalBox());
}

bool Physics::narrowPhase(PhysicsBody& body, PhysicsBody& other)
{
	assert(body.getShape() != nullptr && other.getShape() != nullptr);
	Shape& shape = *body.getShape();
	Shape& otherShape = *other.getShape();

	if (shape.getType() == ShapeType::Sphere && otherShape.getType() == ShapeType::Sphere) {
		return sphereOnSphere(body, other);
	}

	assert(false);
	return false;
}

bool Physics::sphereOnSphere(PhysicsBody& body, PhysicsBody& other)
{
	SphereShape* pSphere = dynamic_cast<SphereShape*>(body.getShape());
	SphereShape* pOtherSphere = dynamic_cast<SphereShape*>(other.getShape());

	if (pSphere == nullptr || pOtherSphere == nullptr) {
		assert(false);
		return false;
	}

	Vector3 toVec = body.getPosition() - other.getPosition();
	
	float radiusSum = pSphere->getRadius() + pOtherSphere->getRadius();

	if (toVec.magnitude() < radiusSum) {
		return true;
	}
	return false;
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

