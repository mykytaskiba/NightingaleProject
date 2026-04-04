#pragma once
#include "vector.h"

class IPropertyVisitor;

class AxisAlignedBox {
public:

	Vector3& min() { return m_min; };
	Vector3& max() { return m_max; };
	Vector3 const& min() const { return m_min; };
	Vector3 const& max() const { return m_max; };
	Vector3 center() const;
	Vector3 extents() const;

	AxisAlignedBox() = default;
	AxisAlignedBox(Vector3 position, Vector3 extends);

	void setPositionExtents(Vector3 const& position, Vector3 const& extends);
	void setPoints(Vector3 const& pointA, Vector3 const& pointB);

	void translate(Vector3 const& translation);

	bool isOverlap(AxisAlignedBox const& other) const;

	void properties(IPropertyVisitor& visitor);

private:
	//Default is a unit box around 0,0,0
	Vector3 m_min{ -0.5f,-0.5f,-0.5f };
	Vector3 m_max{ 0.5f,0.5f,0.5f };
};