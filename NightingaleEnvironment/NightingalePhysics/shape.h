#pragma once

enum class ShapeType {
	Box = 0,
};

class Shape {
protected:
	ShapeType const m_type;
	Shape(ShapeType type) : m_type(type) {}
};
