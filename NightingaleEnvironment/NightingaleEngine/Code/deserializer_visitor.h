#pragma once
#include "json.hpp"
#include "properties.h"
#include "ngmath.h"

struct JSONDeserializerVisitor : public IPropertyVisitor {
	nlohmann::json const& m_json;

	JSONDeserializerVisitor(nlohmann::json const& json) : m_json(json) {}

	template<typename TValue>
	void visit(std::string const& key, TValue& value, MetaData const& metaData) {
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

	//DEFINE OVERRIDE FUNCTIONS
#define X(TType) \
	void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	//END																									

	PROPERTY_TYPES
#undef X
};