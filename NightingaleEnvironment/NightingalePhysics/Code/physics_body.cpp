#include "physics_body.h"
#include "property_visitor.h"
#include "ngjson.h"

JSON_IMPL(PhysicsBody)

void PhysicsBody::update(TTimePhys deltaT)
{
	m_position += m_velocity * deltaT;

	Quaternion deltaVelocity = 0.5f * Quaternion(0, m_angularVelocity[0], m_angularVelocity[1], m_angularVelocity[2]) * m_rotation;
	m_rotation += deltaVelocity * deltaT;
	m_rotation.normalizeEquals();

	calculateGlobalBox();
}

void PhysicsBody::calculateGlobalBox()
{
	m_globalBox = m_localBox;
	m_globalBox.translate(m_position);
}

void PhysicsBody::setMass(float mass)
{
	m_mass = mass;
	if (m_mass <= 0.0f) {
		m_mass = 0.01f;
	}
}

void PhysicsBody::addImpulse(Vector3 const& impulse)
{
	m_velocity += impulse / m_mass;
}

void PhysicsBody::addAngularImpulse(Vector3 const& angularImpulse) {
	m_angularVelocity += angularImpulse;
}

void PhysicsBody::setPosition(Vector3 const& position) {
	m_position = position;
	calculateGlobalBox();
}

void PhysicsBody::resolveCollision(Collision const& collision)
{
	m_position += collision.m_normal * collision.m_depth * 0.5f;
}

Matrix3x3 PhysicsBody::getMomentOfInertiaInverse() const
{
	Matrix3x3 matrix;
	matrix.make_identity();
	if (m_pShape != nullptr) {
		m_pShape->calculateMomentOfInertia(matrix, m_rotation, m_mass);
		return matrix;
	}
	assert(false);
	return matrix;
}

void PhysicsBody::setLocalBox(AxisAlignedBox const& localBox) {
	m_localBox = localBox;
	calculateGlobalBox();
}