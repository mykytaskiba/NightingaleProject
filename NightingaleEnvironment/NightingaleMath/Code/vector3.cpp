#include "vector3.h"
#include <cmath>

Vector3::Vector3(float inX, float inY, float inZ)
{
	data[0] = inX;
	data[1] = inY;
	data[2] = inZ;
}

Vector3& Vector3::operator=(Vector3 const& other) {
	x = other.x;
	y = other.y;
	z = other.z;
	return *this;
}

float& Vector3::operator[](uint index)
{
	//TODO: ASSERT HERE
	return data[index];
}

float Vector3::operator[](uint index) const
{
	//TODO: ASSERT HERE
	return data[index];
}

Vector3& Vector3::operator+=(Vector3 const& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Vector3& Vector3::operator-=(Vector3 const& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}



Vector3& Vector3::operator*=(float scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	return *this;
}

Vector3 Vector3::operator*(float scalar) const
{
	Vector3 result{ *this };
	result *= scalar;
	return result;
}

Vector3& Vector3::operator/=(float scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	return *this;
}

Vector3 Vector3::operator/(float scalar) const
{
	Vector3 result{ *this };
	result /= scalar;
	return result;
}

Vector3 Vector3::operator+(Vector3 const& other)
{
	Vector3 result{ *this };
	result += other;
	return result;
}

Vector3 Vector3::operator-(Vector3 const& other)
{
	Vector3 result{ *this };
	result -= other;
	return result;
}

float Vector3::magnitude_squared() const
{
	return dot(*this);
}

float Vector3::magnitude() const
{
	return sqrt(magnitude_squared());
}

Vector3& Vector3::normalize_equal()
{
	//TO DO: ASSERT magnitude > 0?
	float magn = magnitude();
	if (magn == 0.0f) {
		return *this;
	}
	operator/=(magn);
	return *this;
}

Vector3 Vector3::normalized() const
{
	Vector3 result{*this};
	result.normalize_equal();
	return result;
}

float Vector3::dot(Vector3 const& other) const
{
	return x * other.x + y * other.y + z * other.z;
}

Vector3 Vector3::cross(Vector3 const& other) const
{
	Vector3 result;
	result.x = data[1] * other.data[2] - data[2] * other.data[1];
	result.y = data[2] * other.data[0] - data[0] * other.data[2];
	result.z = data[0] * other.data[1] - data[1] * other.data[0];
	return result;
}

Vector3 Vector3::interpolate(Vector3 const& a, Vector3 const& b, float t)
{
	Vector3 result;
	float oneMinusT = 1.0f - t;
	
	result.x = a.x * oneMinusT + b.x * t;
	result.y = a.y * oneMinusT + b.y * t;
	result.z = a.z * oneMinusT + b.z * t;

	return result;
}
