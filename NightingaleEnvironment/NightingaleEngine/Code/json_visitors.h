#pragma once
#include "json.hpp"
#include "properties.h"

class GameObject;
class IJSONObject;
class Quaternion;
class Transform;

template < typename T, uint DIM >
struct Vector;

struct JSONSerializerVisitor : public IPropertyVisitor {
	nlohmann::json& m_json;

	JSONSerializerVisitor(nlohmann::json& json) : m_json(json) {}

	template<typename TValue>
	void visit(std::string const& key, TValue& value, MetaData const& metaData);

	template<typename TValue>
	void visit(std::string const& key, TValue*& pValue, MetaData const& metaData);

	//DEFINE OVERRIDE FUNCTIONS
	#define X(TType) \
	void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	//END																									

	PROPERTY_TYPES
	#undef X

};


struct JSONDeserializerVisitor : public IPropertyVisitor {
	nlohmann::json const& m_json;

	JSONDeserializerVisitor(nlohmann::json const& json) : m_json(json) {}

	template<typename TValue>
	void visit(std::string const& key, TValue& value, MetaData const& metaData);

	template<typename TValue>
	void visit(std::string const& key, TValue*& value, MetaData const& metaData) {
	}

	//DEFINE OVERRIDE FUNCTIONS
#define X(TType) \
	void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	//END																									

	PROPERTY_TYPES
#undef X
};

#include "json_visitors.tpp"

