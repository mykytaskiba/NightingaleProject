#pragma once
#include "ngmath.h"

class Quaternion {

public:
    float x, y, z, w;

public:
    Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);

    Quaternion(Vector3 axis, float angle);

    Quaternion(Vector3 from, Vector3 to);

    void normalizeEquals();
    float magnitude() const;
    Matrix4x4 getRotation() const; 

    float dot(Quaternion const&) const;

    static Quaternion interpolate(Quaternion const& a, Quaternion const& b, float t);

    Quaternion operator*(float scalar) const;
    Quaternion& operator*=(float scalar);

    Quaternion operator*(Quaternion const&) const;
    Quaternion& operator*=(Quaternion const&);

    Quaternion& operator+=(Quaternion const&);
    Quaternion operator+(Quaternion const&) const;

    Vector3 rotate(Vector3 const&) const;

    bool operator==(Quaternion const&) const;
};

static Quaternion operator*(float scalar, Quaternion const& quat);