#pragma once
#include "property_provider.h"
#include "property_visitor.h"
#include "factory.h"
#include "matrix.h"

enum class ShapeType {
	Empty = 0,
	Box = 1,
	Sphere = 2
};

class Shape : public IPropertyProvider, public IFactoryElement {
protected:
	ShapeType const m_type;
	Shape(ShapeType type) : m_type(type) {}

public:
	void properties(IPropertyVisitor& visitor) override {
	}

	virtual void calculateMomentOfInertia(Matrix3x3& momentInertiaInverse, Quaternion const& rotation, float mass) {}
	ShapeType getType() { return m_type; }
};
