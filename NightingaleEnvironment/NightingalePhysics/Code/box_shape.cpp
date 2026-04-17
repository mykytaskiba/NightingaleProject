#include "box_shape.h"

//STATIC HACK!!
//TO DO: GET RID OF THIS!!

Matrix3x3 Mat4ToMat3(Matrix4x4 const& m) {
	Matrix3x3 result{};
	result[0][0] = m[0][0];
	result[0][1] = m[0][1];
	result[0][2] = m[0][2];

	result[1][0] = m[1][0];
	result[1][1] = m[1][1];
	result[1][2] = m[1][2];

	result[2][0] = m[2][0];
	result[2][1] = m[2][1];
	result[2][2] = m[2][2];
	
	return result;

}

void BoxShape::calculateMomentOfInertia(Matrix3x3& momentInertiaInverse, Quaternion const& rotation, float mass)
{
	float w = m_halfSize[0] * 2.0f;
	float h = m_halfSize[1] * 2.0f;
	float d = m_halfSize[2] * 2.0f;

	float Ixx = 1.0f / 12.0f * mass * (h * h + d * d);
	float Iyy = 1.0f / 12.0f * mass * (w * w + d * d);
	float Izz = 1.0f / 12.0f * mass * (w * w + h * h);

	momentInertiaInverse.make_identity();
	momentInertiaInverse[0][0] = 1.0f/Ixx;
	momentInertiaInverse[1][1] = 1.0f/Iyy;
	momentInertiaInverse[2][2] = 1.0f/Izz;

	Matrix3x3 R = Mat4ToMat3(rotation.getRotation());
	Matrix3x3 Rinv = R.transpose();

	momentInertiaInverse = R * momentInertiaInverse * Rinv;
}

void BoxShape::getAxisFromRotation(Quaternion const& rotation, Vector3& axis1, Vector3& axis2, Vector3& axis3) const
{
	axis1 = rotation.rotate({ 1.0f,0.0f,0.0f });
	axis2 = rotation.rotate({ 0.0f,1.0f,0.0f });
	axis3 = rotation.rotate({ 0.0f,0.0f,1.0f });
}
