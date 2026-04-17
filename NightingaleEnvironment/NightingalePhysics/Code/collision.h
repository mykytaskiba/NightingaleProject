#pragma once
#include "vector.h"

struct Collision {
	
	//no collision!
	Collision() : m_bCollision(false), m_point({ 0.0f,0.0f,0.0f }), m_normal({ 0.0f,0.0f,0.0f }), m_depth(0.0f) {}
	Collision(Vector3 const& point, Vector3 const& normal, float depth) : m_bCollision(true), m_point(point), m_normal(normal), m_depth(depth) {}

	bool m_bCollision;
	Vector3 m_normal;
	Vector3 m_point;
	float m_depth;

};