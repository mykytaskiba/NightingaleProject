#pragma once
#include "ngjson.h" 
#include "ngmath.h"
#include "game_object.h"

template<typename TValue>
void JSONSerializerVisitor::visit(std::string const& key, TValue& value, MetaData const& metaData)
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
void JSONSerializerVisitor::visit(std::string const& key, TValue*& pValue, MetaData const& metaData) {
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

template<typename TValue>
inline void JSONDeserializerVisitor::visit(std::string const& key, TValue& value, MetaData const& metaData)
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