#include "sphere_shape.h"

void SphereShape::calculateMomentOfInertia(Matrix3x3& momentInertiaInverse, Quaternion const& rotation, float mass)
{
	float inertia = 2.0f / 5.0f * mass * m_radius * m_radius;
	inertia = 1.0f / inertia;
	momentInertiaInverse.make_identity();
	
	momentInertiaInverse[0][0] = inertia;
	momentInertiaInverse[1][1] = inertia;
	momentInertiaInverse[2][2] = inertia;
}
