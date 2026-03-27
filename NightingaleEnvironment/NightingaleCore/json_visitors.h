#pragma once
#include "factory.h"
#include "json.hpp"
#include "json_key_constants.h"
#include "property_provider.h"
#include "property_visitor.h"
#include "service_locator.h"
#include <vector>

struct JSONSerializerVisitor : public IPropertyVisitor {
	nlohmann::json& m_json;

	std::vector<nlohmann::json*> m_jsonStack;

	JSONSerializerVisitor(nlohmann::json& json) : m_json(json) { m_jsonStack.push_back(&m_json); }

	void operator()(std::string const& key, int& value, MetaData const& metaData = {}) override {
		json()[key] = value;
	}
	void operator()(std::string const& key, float& value, MetaData const& metaData = {}) override {
		json()[key] = value;
	}
	void operator()(std::string const& key, unsigned int& value, MetaData const& metaData = {}) override {
		json()[key] = value;
	}
	void operator()(std::string const& key, std::string& value, MetaData const& metaData = {}) override {
		json()[key] = value;
	}

protected:
	nlohmann::json& json() {
		return *m_jsonStack.back();
	}

	bool enterScope(std::string const& key, MetaData const& metaData) override {
		m_jsonStack.push_back(&json()[key]);
		return true;
	}
	void leaveScope(MetaData const& metaData) override {
		m_jsonStack.pop_back();
	}

	bool enterFactory(std::string const& key, void*& pValue, MetaData const& metaData) {
		return false;
		//if (pValue == nullptr) {
		//	json()[key] = nullptr;
		//	return;
		//}

		//pValue->json()->serialize(json);
	}

};


struct JSONDeserializerVisitor : public IPropertyVisitor {

	/*
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
	}*/


	nlohmann::json& m_json;

	std::vector<nlohmann::json*> m_jsonStack; 

	JSONDeserializerVisitor(nlohmann::json& json) : m_json(json) { m_jsonStack.push_back(&m_json); }

	void operator()(std::string const& key, int& value, MetaData const& metaData = {}) override {
		if (!json().contains(key)) return;
 		json()[key].get_to(value);
	}
	void operator()(std::string const& key, float& value, MetaData const& metaData = {}) override {
		if (!json().contains(key)) return;
		json()[key].get_to(value);
	}
	void operator()(std::string const& key, unsigned int& value, MetaData const& metaData = {}) override {
		if (!json().contains(key)) return;
		json()[key].get_to(value);
	}
	void operator()(std::string const& key, std::string& value, MetaData const& metaData = {}) override {
		if (!json().contains(key)) return;
		json()[key].get_to(value);
	}


protected:
	nlohmann::json& json() {
		return *m_jsonStack.back();
	}

	bool enterScope(std::string const& key, MetaData const& metaData) override {
		if (!json().contains(key)) {
			return false;
		}
		m_jsonStack.push_back(&json()[key]);
	}
	void leaveScope(MetaData const& metaData) override {
		m_jsonStack.pop_back();
	}

	bool enterFactory(std::string const& key, void*& pValue, MetaData const& metaData) {
		return false;
	}
};

