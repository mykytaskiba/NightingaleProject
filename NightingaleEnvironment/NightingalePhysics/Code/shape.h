#pragma once
#include "property_provider.h"
#include "property_visitor.h"
#include "factory.h"

enum class ShapeType {
	Empty = 0,
	Box = 1,
	Sphere = 2
};

class Shape : public IPropertyProvider, public IFactoryElement {
protected:
	ShapeType const m_type;
	Shape(ShapeType type) : m_type(type) {}

	void properties(IPropertyVisitor& visitor) override {
	}

public:
	ShapeType getType() { return m_type; }
};
