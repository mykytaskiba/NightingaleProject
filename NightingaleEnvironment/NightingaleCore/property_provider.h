#pragma once
#include "property_metadata.h"

class IPropertyVisitor;

class IPropertyProvider {
public:
	virtual void properties(IPropertyVisitor& visitor) = 0;
};

template <typename TValue>
concept HasProperties = requires(TValue value, IPropertyVisitor visitor) {
	value.properties(visitor);
};