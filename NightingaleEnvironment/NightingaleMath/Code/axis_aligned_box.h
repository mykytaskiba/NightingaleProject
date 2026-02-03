#pragma once
#include "vector.h"

class AxisAlignedBox {
public:

	Vector3 const& min() const;
	Vector3 const& max() const;
	Vector3 center() const;
	Vector3 extends() const;

	AxisAlignedBox() = default;
	AxisAlignedBox(Vector3 position, Vector3 extends);

	void setPositionExtends(Vector3 const& position, Vector3 const& extends);
	void setPoints(Vector3 const& pointA, Vector3 const& pointB);

	void translate(Vector3 const& translation);

	bool isOverlap(AxisAlignedBox const& other) const;

private:
	Vector3 m_min{ 0.0f,0.0f,0.0f };
	Vector3 m_max{ 0.0f,0.0f,0.0f };
};