#pragma once
#include "json.hpp"
#include "properties.h"
#include "ngjson.h" 
#include "ngmath.h"
#include "game_object.h"
#include "engine_functions.h"

template < typename T, uint DIM >
struct Vector;

struct JSONSerializerVisitor : public IPropertyVisitor {
	nlohmann::json& m_json;

	JSONSerializerVisitor(nlohmann::json& json) : m_json(json) {}

	template<typename TValue>
	void visit(std::string const& key, TValue& value, MetaData const& metaData)
	{
		if constexpr (HasProperties<TValue>) {
			JSONSerializerVisitor childSerializer{ m_json[key] };
			value.properties(childSerializer);
		}
		else {
			m_json[key] = value;
		}
	}

	template<typename TValue>
	void visit(std::string const& key, TValue*& pValue, MetaData const& metaData)
	{
		if (metaData.m_bReadOnly) {
			return;
		}
		if (pValue == nullptr) {
			m_json[key] = nullptr;
			return;
		}

		nlohmann::json& jsonValue = m_json[key];
		pValue->json()->serialize(jsonValue);
	}

	//DEFINE OVERRIDE FUNCTIONS
	#define X(TType) \
	void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	//END																									

	PROPERTY_TYPES
	#undef X

};


struct JSONDeserializerVisitor : public IPropertyVisitor {
	nlohmann::json& m_json;

	JSONDeserializerVisitor(nlohmann::json& json) : m_json(json) {}

	template<typename TValue>
	void visit(std::string const& key, TValue& value, MetaData const& metaData)
	{
		if (metaData.m_bReadOnly) {
			return;
		}
		if constexpr (HasProperties<TValue>) {
			if (m_json.contains(key)) {
				JSONDeserializerVisitor childSerializer{ m_json[key] };
				value.properties(childSerializer);
			}
		}
		else {
			if (m_json.contains(key)) {
				value = m_json[key].get<TValue>();
			}
		}
	}

	template<>
	void visit(std::string const& key, GameObject*& pValue, MetaData const& metaData) {
		if (pValue != nullptr) {
			//return;
			pValue = nullptr;
		}

		if (!m_json[key].contains("factory_key")) {
			return;
		}

		std::string factoryKey = m_json[key]["factory_key"];
		if (EngineFunctions::factoryGameObject().create(factoryKey, pValue)) {
			pValue->json()->deserialize(m_json[key]);
		}
	}

	//DEFINE OVERRIDE FUNCTIONS
#define X(TType) \
	void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	//END																									

	PROPERTY_TYPES
#undef X
};

