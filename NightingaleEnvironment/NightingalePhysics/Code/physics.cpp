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

	Collision collision = narrowPhase(body, other);
	if (!collision.m_bCollision) {
		return;
	}

	//Sink correction
	body.setPosition( body.getPosition() + (collision.m_normal * collision.m_depth * 0.5f) );
	other.setPosition(other.getPosition() - (collision.m_normal * collision.m_depth * 0.5f) );
	
	Matrix3x3 invInertia = body.getMomentOfInertiaInverse();
	Matrix3x3 invInertiaOther = other.getMomentOfInertiaInverse();

	Vector3 relativePoint = collision.m_point - body.getPosition();
	Vector3 relativePointOther = collision.m_point - other.getPosition();

	Vector3 pointVelocity = body.getVelocity() + body.getAngularVelocity().cross(relativePoint);
	Vector3 pointVelocityOther = other.getVelocity() + other.getAngularVelocity().cross(relativePointOther);

	Vector3 relativeVelocity = pointVelocity - pointVelocityOther;

	//impulse scalar
	float velocityAlongNormal = (relativeVelocity).dot(collision.m_normal);
	if (velocityAlongNormal > 0.0f) return;
	
	float restitution = m_restitution; //TO DO: currently using a global value

	float angularTermA = (invInertia * relativePoint.cross(collision.m_normal)).cross(relativePoint).dot(collision.m_normal);
	float angularTermB = (invInertiaOther * relativePointOther.cross(collision.m_normal)).cross(relativePointOther).dot(collision.m_normal);

	float j = -(1.0f + restitution) * velocityAlongNormal;
	j /= (1.0f / body.getMass()) + (1.0f / other.getMass()) + angularTermA + angularTermB;

	Vector3 impulse = j * collision.m_normal;
	body.addImpulse(impulse);
	body.addAngularImpulse(invInertia * relativePoint.cross(impulse));

	other.addImpulse(impulse * -1.0f);
	other.addAngularImpulse(invInertia * relativePoint.cross(impulse * -1.0f));


	//body.setVelocity(Vector3(0, 0, 0));
	//other.setVelocity(Vector3(0, 0, 0));
}

bool Physics::broadPhase(PhysicsBody& body, PhysicsBody& other)
{
	return body.getGlobalBox().isOverlap(other.getGlobalBox());
}

Collision Physics::narrowPhase(PhysicsBody& body, PhysicsBody& other)
{
	assert(body.getShape() != nullptr && other.getShape() != nullptr);
	Shape& shape = *body.getShape();
	Shape& otherShape = *other.getShape();

	if (shape.getType() == ShapeType::Sphere && otherShape.getType() == ShapeType::Sphere) {
		return sphereOnSphere(body, other);
	}

	assert(false);
	return Collision();
}

Collision Physics::sphereOnSphere(PhysicsBody& body, PhysicsBody& other)
{
	SphereShape* pSphere = dynamic_cast<SphereShape*>(body.getShape());
	SphereShape* pOtherSphere = dynamic_cast<SphereShape*>(other.getShape());

	if (pSphere == nullptr || pOtherSphere == nullptr) {
		assert(false);
		return Collision();
	}

	Vector3 toVec = body.getPosition() - other.getPosition();
	
	float radiusSum = pSphere->getRadius() + pOtherSphere->getRadius();

	if (toVec.magnitude() < radiusSum) {
		Vector3 collisionPoint = body.getPosition() + other.getPosition() + toVec.normalized() * (pSphere->getRadius() + -1.0f * pOtherSphere->getRadius());
		collisionPoint /= 2.0f;
		return Collision(collisionPoint, toVec.normalized(), radiusSum - toVec.magnitude());
	}
	return Collision();
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

