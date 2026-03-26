#include "physics_body.h"
#include "property_visitor.h"
#include "ngjson.h"

JSON_IMPL(PhysicsBody)

void PhysicsBody::update(TTimePhys deltaT)
{
	m_position += m_velocity * deltaT;
	calculateGlobalBox();
}

void PhysicsBody::calculateGlobalBox()
{
	m_globalBox = m_localBox;
	m_globalBox.translate(m_position);
}

void PhysicsBody::setPosition(Vector3 const& position) {
	m_position = position;
	calculateGlobalBox();
}

void PhysicsBody::setLocalBox(AxisAlignedBox const& localBox) {
	m_localBox = localBox;
	calculateGlobalBox();
}