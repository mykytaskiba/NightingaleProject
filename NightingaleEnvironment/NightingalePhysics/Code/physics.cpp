#include "physics.h"
#include "nightingale_assert.h"
#include <iostream>
#include "box_shape.h"

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
		[this, deltaT](PhysicsBody& body, PhysicsBody& other) {
			resolvePossibleCollision(body, other, deltaT);
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

void Physics::resolvePossibleCollision(PhysicsBody& body, PhysicsBody& other, TTimePhys deltaT)
{
	++m_infoSpatialPairsCount;

	if (!body.hasShape() || !other.hasShape()) {
		//TO DO: would this ever be the case?
		return;
	}

	if (body.isImmovable() && other.isImmovable()) {
		//TO DO: would this ever be the case?
		return;
	}


	if (!broadPhase(body, other)) {
		return;
	}

	Collision collision = narrowPhase(body, other);
	if (!collision.m_bCollision) {
		return;
	}

	float correctionFactor = 0.5f;
	if (body.isImmovable()) {
		correctionFactor = 0.0f;
	}
	else if (other.isImmovable()) {
		correctionFactor = 1.0f;
	}

	//Sink correction
	body.setPosition(body.getPosition() + (collision.m_normal * collision.m_depth * correctionFactor));
	other.setPosition(other.getPosition() - (collision.m_normal * collision.m_depth * (1.0f - correctionFactor)));
	
	Matrix3x3 invInertia = body.getMomentOfInertiaInverse();
	Matrix3x3 invInertiaOther = other.getMomentOfInertiaInverse();

	Vector3 relativePoint = collision.m_point - body.getPosition();
	Vector3 relativePointOther = collision.m_point - other.getPosition();

	Vector3 pointVelocity = body.getVelocity() + body.getAngularVelocity().cross(relativePoint);
	Vector3 pointVelocityOther = other.getVelocity() + other.getAngularVelocity().cross(relativePointOther);

	Vector3 relativeVelocity = pointVelocity - pointVelocityOther;

	float restitution = m_restitution; //TO DO: currently using a global value

	//impulse scalar
	float velocityAlongNormal = (relativeVelocity).dot(collision.m_normal);
	if (velocityAlongNormal > 0.0f) {
		return;
	}
	else {
		if (collision.m_depth > 0.02f) {
			restitution = 0.0f;
		}
	}
	

	float angularTermA = (invInertia * relativePoint.cross(collision.m_normal)).cross(relativePoint).dot(collision.m_normal);
	float angularTermB = (invInertiaOther * relativePointOther.cross(collision.m_normal)).cross(relativePointOther).dot(collision.m_normal);
	if (!m_bEnableRotationCalculations) {
		angularTermA = 0.0f;
		angularTermB = 0.0f;
	}

	float invAMass = body.isImmovable() ? 0.0f : 1.0f / body.getMass();
	float invBMass = other.isImmovable() ? 0.0f : 1.0f / other.getMass();

	float slop = 0.01f;
	float bias = 0.1f;
	float baumgarteBias = bias / deltaT * std::max(0.0f, collision.m_depth - slop);

	float j = -(1.0f + restitution) * velocityAlongNormal + baumgarteBias;


	j /= invAMass + invBMass + angularTermA + angularTermB;

	Vector3 impulse = j * collision.m_normal;

	if (!body.isImmovable()) {
		body.addImpulse(impulse);
		if (m_bEnableRotationCalculations) body.addAngularImpulse(invInertia * relativePoint.cross(impulse));

		if (m_bInfoCollectCollisionPoints) {
			InfoCollisionPoint infoPoint{};
			infoPoint.point = collision.m_point;
			infoPoint.force = impulse;
			collisionPoints.push_back(infoPoint);
		}
	}

	if (!other.isImmovable()) {
		other.addImpulse(-1.0f * impulse);
		if (m_bEnableRotationCalculations) other.addAngularImpulse((invInertiaOther * relativePointOther.cross(-1.0f * impulse)));

		if (m_bInfoCollectCollisionPoints) {
			InfoCollisionPoint infoPoint{};
			infoPoint.point = collision.m_point;
			infoPoint.force = -1.0f * impulse;
			collisionPoints.push_back(infoPoint);
		}
	}


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
	if (shape.getType() == ShapeType::Box && otherShape.getType() == ShapeType::Box) {
		return boxOnBox(body, other);
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

Vector3 GetSupportPoint(Vector3 const& axis1, Vector3 const& axis2, Vector3 const& axis3, Vector3 const& halfSize, Vector3 const& center, const Vector3& direction) {
	Vector3 localDir;
	localDir[0] = direction.dot(axis1);
	localDir[1] = direction.dot(axis2);
	localDir[2] = direction.dot(axis3);
	
	Vector3 resultLocal;
	resultLocal[0] = (localDir[0] > 0) ? halfSize[0] : -halfSize[0];
	resultLocal[1] = (localDir[1] > 0) ? halfSize[1] : -halfSize[1];
	resultLocal[2] = (localDir[2] > 0) ? halfSize[2] : -halfSize[2];

	return center +
		(axis1 * resultLocal[0]) +
		(axis2 * resultLocal[1]) +
		(axis3 * resultLocal[2]);
}

Collision Physics::boxOnBox(PhysicsBody& body, PhysicsBody& other)
{
	BoxShape* pBox = dynamic_cast<BoxShape*>(body.getShape());
	BoxShape* pOtherBox = dynamic_cast<BoxShape*>(other.getShape());

	if (pBox == nullptr || pOtherBox == nullptr) {
		assert(false);
		return Collision();
	}

	float minOverlap = std::numeric_limits<float>::max();
	Vector3 bestAxis{};

	Vector3 axisToTest[15];
	pBox->getAxisFromRotation(body.getRotation(), axisToTest[0], axisToTest[1], axisToTest[2]);
	pOtherBox->getAxisFromRotation(other.getRotation(), axisToTest[3], axisToTest[4], axisToTest[5]);

	unsigned int k{ 6u };
	for (unsigned int i{ 0u }; i < 3; ++i) {
		for (unsigned int j{ 3u }; j < 6; ++j) {
			axisToTest[k++] = axisToTest[i].cross(axisToTest[j]);
		}
	}

	Vector3 toVec = other.getPosition() - body.getPosition();

	for (Vector3 L : axisToTest) {
		if (L.magnitude_squared() < 0.01f) continue; // Skip parallel edges
		L.normalize_equal();


		float rA = pBox->getHalfSize()[0] * abs(axisToTest[0].dot(L)) +
			pBox->getHalfSize()[1] * abs(axisToTest[1].dot(L)) +
			pBox->getHalfSize()[2] * abs(axisToTest[2].dot(L));

		float rB = pOtherBox->getHalfSize()[0] * abs(axisToTest[3].dot(L)) +
			pOtherBox->getHalfSize()[1] * abs(axisToTest[4].dot(L)) +
			pOtherBox->getHalfSize()[2] * abs(axisToTest[5].dot(L));

		
		float centerDist = abs(toVec.dot(L));
		float overlap = (rA + rB) - centerDist;

		if (overlap <= 0) return Collision(); // Found a separating axis!

		if (overlap < minOverlap) {
			minOverlap = overlap;
			bestAxis = L;
		}
	}

	Collision result;
	Vector3 d = other.getPosition()- body.getPosition();
	if (d.dot(bestAxis) < 0) bestAxis = -1.0f * bestAxis; // Ensure normal points A -> B

	result.m_bCollision = true;
	result.m_normal = -1.0f * bestAxis;
	result.m_depth = minOverlap;

	Vector3 point1 = GetSupportPoint(axisToTest[0], axisToTest[1], axisToTest[2], pBox->getHalfSize(), body.getPosition(), bestAxis);
	Vector3 point2 = GetSupportPoint(axisToTest[3], axisToTest[4], axisToTest[5], pOtherBox->getHalfSize(), other.getPosition(), -1.0f * bestAxis);

	result.m_point = (point1 + point2) / 2.0f;
	return result;
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

