#pragma once
#include "ngmath.h"

class IPropertyVisitor;

//a transform class represents a position, scale and a rotation in the world
class Transform {
public:
    Vector3 scale{ 1.0f, 1.0f, 1.0f };
    Vector3 position{ 0.0f,0.0f,0.0f };
    Quaternion rotation{ 0.0f,0.0f,0.0f,1.0f };

    static Transform interpolate(Transform const& a, Transform const& b, float t);

    Matrix4x4 getMatrix() const;

    Vector3 transform(Vector3 const&) const;


    void properties(IPropertyVisitor& visitor);

};