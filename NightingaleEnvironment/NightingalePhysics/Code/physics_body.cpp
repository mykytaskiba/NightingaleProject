#include "physics_body.h"

void PhysicsBody::update(TTimePhys deltaT)
{
	position += velocity * deltaT;

	globalBox = localBox;
	globalBox.translate(position);
}
