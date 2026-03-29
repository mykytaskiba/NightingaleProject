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

	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData) override {
		if (pValue == nullptr) {
			json()[key] = nlohmann::json();
			return;
		}

		IJSONObject* pJSONObject = dynamic_cast<IJSONObject*>(pValue);
		if (pJSONObject == nullptr) {
			assert(false);
			return;
		}

		bool bSuccess = pJSONObject->json()->serialize(json()[key]);
		assert(bSuccess);
	}

};


struct JSONDeserializerVisitor : public IPropertyVisitor {

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

	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData) override {

	}
};

