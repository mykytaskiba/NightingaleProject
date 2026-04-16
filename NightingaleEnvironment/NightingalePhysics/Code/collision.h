#pragma once
#include "vector.h"

struct Collision {
	
	//no collision!
	Collision() : m_bCollision(false), m_normal({ 0.0f,0.0f,0.0f }), m_depth(0.0f) {}
	Collision(Vector3 const& normal, float depth) : m_bCollision(true), m_normal(normal), m_depth(depth) {}

	bool m_bCollision;
	Vector3 m_normal;
	float m_depth;

};