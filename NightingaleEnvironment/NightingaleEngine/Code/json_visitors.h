#pragma once
#include "factory.h"
#include "json.hpp"
#include "json_key_constants.h"
#include "property_provider.h"
#include "properties.h"
#include "service_locator.h"

struct JSONSerializerVisitor : public IPropertyVisitor {
	nlohmann::json& m_json;

	JSONSerializerVisitor(nlohmann::json& json) : m_json(json) {}


	template<typename TValue>
	void visit_internal(nlohmann::json& json, TValue& value, MetaData const& metaData) {
		if constexpr (HasProperties<TValue>) {
			JSONSerializerVisitor childSerializer{ json };
			value.properties(childSerializer);
		}
		else {
			json = value;
		}
	}

	template<typename TValue>
	void visit_internal(nlohmann::json& json, TValue*& pValue, MetaData const& metaData) {
		if (pValue == nullptr) {
			json = nullptr;
			return;
		}

		pValue->json()->serialize(json);
	}

	template<typename TValue>
	void visit(std::string const& key, TValue& value, MetaData const& metaData)
	{
		visit_internal(m_json[key], value, metaData);
	}

	template<typename TValue>
	void visit(std::string const& key, std::vector<TValue>& collection, MetaData const& metaData)
	{
		m_json[key] = nlohmann::json::array();
		nlohmann::json& jsonArray = m_json[key];

		for (auto& value : collection) {
			nlohmann::json& serializedObject = jsonArray.emplace_back(nlohmann::json::value_t::object);
			visit_internal(serializedObject, value, metaData);
		}
	}

	//DEFINE OVERRIDE FUNCTIONS
#define X(TType) \
	void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	void operator()(std::string const& key, std::vector<TType>& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	//END																									

	PROPERTY_TYPES
#undef X

};


struct JSONDeserializerVisitor : public IPropertyVisitor {
	nlohmann::json& m_json;

	JSONDeserializerVisitor(nlohmann::json& json) : m_json(json) {}


	template<typename TValue>
	void visit_internal(nlohmann::json& json, TValue& value, MetaData const& metaData) {
		if constexpr (HasProperties<TValue>) {
			JSONDeserializerVisitor childSerializer{ json };
			value.properties(childSerializer);
		}
		else {
			json.get_to(value);
		}
	}

	template<typename TValue>
	void visit_internal(nlohmann::json& json, TValue*& pValue, MetaData const& metaData) {
		if (pValue != nullptr) {
			//return; //TO DO: THIS CREATES A MEMORY LEAK DEAL WITH THIS LATER
			pValue = nullptr;
		}
		if (!json.contains(JSON_META_KEY)) {
			return;
		}
		nlohmann::json& jsonMeta = json[JSON_META_KEY];
		if (!json.contains(JSON_FACTORY_KEY)) {
			return;
		}

		std::string factoryKey = jsonMeta[JSON_FACTORY_KEY];
		if (!ServiceLocator<Factory<std::string, TValue>>::hasService()) {
			return;
		}
		Factory<std::string, TValue>& factory = *ServiceLocator< Factory<std::string, TValue> >::retrieve();

		if (factory.create(factoryKey, pValue)) {
			pValue->json()->deserialize(json);
		}
	}

	template<typename TValue>
	void visit(std::string const& key, TValue& value, MetaData const& metaData)
	{
		if (metaData.m_bReadOnly) {
			return;
		}
		if (m_json.contains(key)) {
			visit_internal(m_json[key], value, metaData);
		}
	}

	template<typename TValue>
	void visit(std::string const& key, std::vector<TValue>& collection, MetaData const& metaData)
	{
		if (!m_json.contains(key) || !m_json[key].is_array()) {
			return;
		}
		if (!collection.empty()) {
			//return; TO DO: FIX THIS LATER
			collection.clear();
		}

		nlohmann::json& jsonArray = m_json[key];

		collection.resize(jsonArray.size());

		for (size_t i = 0; i < jsonArray.size(); ++i) {
			visit_internal(jsonArray[i], collection[i], metaData);
		}
	}

	//DEFINE OVERRIDE FUNCTIONS
#define X(TType) \
	void operator()(std::string const& key, TType& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	void operator()(std::string const& key, std::vector<TType>& value, MetaData const& metaData = {}) override	\
	{ visit(key, value, metaData); } \
	//END																									

	PROPERTY_TYPES
#undef X
};

