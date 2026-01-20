#include "pch.h"
#include "quaternion.h"
#include "vector.h"

Quaternion::Quaternion(float inX, float inY, float inZ, float inW) 
    : x(inX), y(inY), z(inZ), w(inW)
{
}

Quaternion::Quaternion(Vector3 axis, float angle) : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
   // assert(axis.magnitude() == 1.0f);
    
    float sin_a = sin(angle / 2.0f);
    w = cos(angle/2.0f);

    x = axis[0] * sin_a;
    y = axis[1] * sin_a;
    z = axis[2] * sin_a;

}

Quaternion::Quaternion(Vector3 from, Vector3 to) : x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
    w = sqrt(from.magnitude_squared() * to.magnitude_squared()) + from.dot(to);
    Vector3 cross = from.cross(to);
    x = cross[0];
    y = cross[1];
    z = cross[2];

    normalizeEquals();
}

void Quaternion::normalizeEquals()
{
    float magn = magnitude();

    x /= magn;
    y /= magn;
    z /= magn;
    w /= magn;
}

float Quaternion::magnitude() const
{
    return sqrt(x * x + y * y + z * z + w * w);
}

Matrix4x4 Quaternion::getRotation() const
{
    //assert(magnitude() == 1.0f);

    Matrix4x4 result = Matrix4x4::Identity();
    result(0, 0) = 1.0f - 2.0f * y * y - 2.0f * z * z;
    result(1, 0) = 2.0f * x * y + 2.0f * z * w;
    result(2, 0) = 2.0f * x * z - 2.0f * y * w;

    result(0, 1) = 2.0f * x * y - 2.0f * z * w;
    result(1, 1) = 1.0f - 2.0f * x * x - 2.0f * z * z;
    result(2, 1) = 2.0f * y * z + 2.0f * x * w;

    result(0, 2) = 2.0f * x * z + 2.0f * y * w;
    result(1, 2) = 2.0f * y * z - 2.0f * x * w;
    result(2, 2) = 1.0f - 2.0f * x * x - 2.0f * y * y;

    result(3, 3) = 1.0f;

    return result;
}

float Quaternion::dot(Quaternion const& other) const
{
    return x * other.x + y * other.y + z * other.z + w * other.w;
}

Quaternion Quaternion::interpolate(Quaternion const& a, Quaternion const& b, float t)
{
    float angle = acos(a.dot(b));
    if (std::isnan(angle)) {
        return a;
    }

    float oneMinusT = 1.0f - t;

    float sinA = sin(angle);
    if (sinA == 0.0f) {
        return a;
    }

    float A = sin(oneMinusT * angle) / sinA;
    float B = sin(t * angle) / sinA;


    Quaternion result = A * a + B * b;
    if (std::isnan(result.x)) {
        bool b = true;
    }
    //return a;
    return result;
}

Quaternion Quaternion::operator*(float scalar) const
{
    Quaternion result(*this);
    result *= scalar;
    return result;
}

Quaternion& Quaternion::operator*=(float scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}

Quaternion Quaternion::operator*(Quaternion const& other) const
{
    Quaternion result;

    
    result.w = w * other.w - x * other.x - y * other.y - z * other.z;
    result.x = w * other.x + x * other.w + y * other.z - z * other.y;
    result.y = w * other.y - x * other.z + y * other.w + z * other.x;
    result.z = w * other.z + x * other.y - y * other.x + z * other.w;

    return result;

    //a.w* b.w - a.x * b.x - a.y * b.y - a.z * b.z,  // 1
    //a.w* b.x + a.x * b.w + a.y * b.z - a.z * b.y,  // i
    //a.w* b.y - a.x * b.z + a.y * b.w + a.z * b.x,  // j
    //a.w* b.z + a.x * b.y - a.y * b.x + a.z * b.w   // k
}

Quaternion& Quaternion::operator*=(Quaternion const& other)
{ 
    Quaternion temp = (*this) * other;
    *this = temp;
    return *this;
}

Quaternion& Quaternion::operator+=(Quaternion const& other)
{

    x += other.x;
    y += other.y;
    z += other.z;
    w += other.w;
    return *this;
}

Quaternion Quaternion::operator+(Quaternion const& other) const
{
    Quaternion result(*this);
    result += other;
    return result;
}

Vector3 Quaternion::rotate(Vector3 const& vector) const
{
    Vector3 result = (getRotation() * vector.add_dimension(1.0f)).truncate_dimension();
    return result;
    //quat a*b multiplicaiton
    //a.w* b.w - a.x * b.x - a.y * b.y - a.z * b.z,  // 1
    //a.w* b.x + a.x * b.w + a.y * b.z - a.z * b.y,  // i
    //a.w* b.y - a.x * b.z + a.y * b.w + a.z * b.x,  // j
    //a.w* b.z + a.x * b.y - a.y * b.x + a.z * b.w   // k

}


bool Quaternion::operator==(Quaternion const& other) const
{
    return (x == other.x &&
        y == other.y &&
        z == other.z &&
        w == other.w);
}

Quaternion operator*(float scalar, Quaternion const& quat)
{
    return quat* scalar;
}
