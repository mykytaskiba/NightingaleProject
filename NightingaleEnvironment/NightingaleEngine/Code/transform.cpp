#include "pch.h"
#include "transform.h"


Transform Transform::interpolate(Transform const& a, Transform const& b, float t)
{
    Transform result;

    result.position = Vector3::interpolate(a.position, b.position, t);
    result.scale = pow(a.scale, (1.0f - t))*pow(b.scale, t);
    result.rotation = Quaternion::interpolate(a.rotation, b.rotation, t);


    return result;
}

Matrix4x4 Transform::getMatrix() const
{
    Matrix4x4 result = rotation.getRotation();

    result(0,3) = position[0];
    result(1, 3) = position[1];
    result(2, 3) = position[2];

    for (uint i = 0; i < 3; ++i) {
        for (uint j = 0; j < 3; ++j) {
            result(i, j) *= scale;
        }
    }
    
    return result;
}

Vector3 Transform::transform(Vector3 const& point) const
{
    return rotation.rotate(point * scale) + position;
}
