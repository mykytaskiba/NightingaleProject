#pragma once
#include <string>
#include "property_metadata.h"

class GameObject;
class PhysicsBody;
class Transform;
struct Quaternion;
template < typename T, unsigned int DIM >
struct Vector;
struct AxisAlignedBox;

using PropVector3 = Vector<float, 3u>;

struct IPropertyVisitor {

#define PROPERTY_TYPES \
	X(float) \
	X(int) \
	X(unsigned int) \
	X(std::string) \
	X(PropVector3) \
	X(Quaternion) \
	X(Transform) \
	X(AxisAlignedBox) \
	X(GameObject*) \
	X(PhysicsBody) \
	//END

#define X(TType) \
	virtual void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) = 0; \
	virtual void operator()(std::string const& key, std::vector<TType>& value, MetaData const& metaData = {}) = 0; \

	//END

	//BUILD ABSTRACT FUNCTIONS
	PROPERTY_TYPES
#undef X


};
