#pragma once
#include "json.hpp"
#include "properties.h"
#include "ngmath.h"

struct JSONSerializerVisitor : public IPropertyVisitor {
	nlohmann::json& m_json;

	JSONSerializerVisitor(nlohmann::json& json) : m_json(json) {}

	template<typename TValue>
	void visit(std::string const& key, TValue& value, MetaData const& metaData) {
		if constexpr (HasProperties<TValue>) {
			JSONSerializerVisitor childSerializer{ m_json[key] };
			value.properties(childSerializer);
		}
		else {
			m_json[key] = value;
		}
	}

	/*
	template<typename TValue>
	requires IsFactoryObject<TValue> && HasJSONRepresentation<TValue>
		void visit(std::string const& key, TValue*& value, MetaData const& metaData) {
		TValue::JSONRepresnetaiton;
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
	}*/

	//DEFINE OVERRIDE FUNCTIONS
	#define X(TType) \
	void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	//END																									

	PROPERTY_TYPES
	#undef X

};