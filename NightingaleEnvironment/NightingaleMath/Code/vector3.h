#pragma once
//Mathmatical vector
#include "defines-ngmath.h"


struct Vector3 {
    float data[3]{ 0.0f,0.0f,0.0f };
    float& x = data[0];
    float& y = data[1];
    float& z = data[2];

    Vector3(Vector3 const& other) = default;
    Vector3() = default;

    Vector3& operator=(Vector3 const& other);

    Vector3(float inX, float inY, float inZ);

    float& operator[](uint index);
    float operator[](uint index) const;

    Vector3& operator+=(Vector3 const& other);
    Vector3& operator-=(Vector3 const& other);

    Vector3 operator+(Vector3 const& other);
    Vector3 operator-(Vector3 const& other);

    Vector3& operator*=(float scalar);
    Vector3& operator/=(float scalar);

    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;


    float magnitude_squared() const;
    float magnitude() const;
    Vector3& normalize_equal();
    Vector3 normalized() const;

    float dot(Vector3 const& other) const;
    Vector3 cross(Vector3 const& other) const;
    static Vector3 interpolate(Vector3 const& a, Vector3 const& b, float t);

};

static Vector3 operator*(float scalar, Vector3 const& vector) {
    return (vector * scalar);
}