#pragma once
#include "ngmath.h"
#include "transform.h"

/*
struct VisitorTemplate {
	template<typename TValue, typename... TMeta >
	void operator()(std::string const& key, TValue& value, TMeta&&... meta);
};
class VisitorAcceptor {
	template <typename TPropertyVisitor>
	void properties(TPropertyVisitor& visitor);
}
*/


struct MetaData {
	bool m_bReadOnly{ false };
};

struct IPropertyVisitor {

#define PROPERTY_TYPES \
	X(float) \
	X(int) \
	X(unsigned int) \
	X(std::string) \
	X(Vector3) \
	X(Quaternion) \
	X(Transform) \
	//END

#define X(TType) \
	virtual void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) = 0; \
	//END

	//BUILD ABSTRACT FUNCTIONS
	PROPERTY_TYPES
#undef X


};

template <typename TObject>
concept HasProperties = requires(TObject obj, IPropertyVisitor visitor) {
	obj.properties(visitor);
};