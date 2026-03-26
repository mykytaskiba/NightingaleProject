#include "axis_aligned_box.h"
#include "defines.h"
#include <cmath>

Vector3 AxisAlignedBox::center() const
{
	return (m_max + m_min) / 2.0f;
}

Vector3 AxisAlignedBox::extents() const
{
	return (m_max - m_min);
}

AxisAlignedBox::AxisAlignedBox(Vector3 position, Vector3 extends)
{
	if (extends[0] < 0.0f) { extends[0] *= -1.0f; }
	if (extends[1] < 0.0f) { extends[1] *= -1.0f; }
	if (extends[2] < 0.0f) { extends[2] *= -1.0f; }
	m_max = position + extends * 0.5f;
	m_min = position - extends * 0.5f;
}

void AxisAlignedBox::setPositionExtents(Vector3 const& position, Vector3 const& extends)
{
	Vector3 extendsCopy{ extends };
	if (extendsCopy[0] < 0.0f) { extendsCopy[0] *= -1.0f; }
	if (extendsCopy[1] < 0.0f) { extendsCopy[1] *= -1.0f; }
	if (extendsCopy[2] < 0.0f) { extendsCopy[2] *= -1.0f; }
	m_max = position + extends * 0.5f;
	m_min = position - extends * 0.5f;
}

void AxisAlignedBox::setPoints(Vector3 const& pointA, Vector3 const& pointB)
{
	m_min[0] = std::min(pointA[0], pointB[0]);
	m_min[1] = std::min(pointA[1], pointB[1]);
	m_min[2] = std::min(pointA[2], pointB[2]);

	m_max[0] = std::max(pointA[0], pointB[0]);
	m_max[1] = std::max(pointA[1], pointB[1]);
	m_max[2] = std::max(pointA[2], pointB[2]);
}

void AxisAlignedBox::translate(Vector3 const& translation) {
	m_min += translation;
	m_max += translation;
}

bool AxisAlignedBox::isOverlap(AxisAlignedBox const& other) const
{
	//TO DO: Come back to this function, could it be written better? 
	if (m_max[0] >= other.m_min[0] && other.m_max[0] >= m_min[0]) {
		if (m_max[1] >= other.m_min[1] && other.m_max[1] >= m_min[1]) {
			if (m_max[2] >= other.m_min[2] && other.m_max[2] >= m_min[2]) {
				return true;
			}
		}
	}
	return false;
}
