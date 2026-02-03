#include "physics_body.h"

void Body::update(TTimePhys deltaT)
{
	position += velocity * deltaT;

	globalBox = localBox;
	globalBox.translate(position);
}
