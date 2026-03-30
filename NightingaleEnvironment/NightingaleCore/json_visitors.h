#pragma once
#include "factory.h"
#include "json.hpp"
#include "json_key_constants.h"
#include "property_provider.h"
#include "property_visitor.h"
#include "service_locator.h"
#include <vector>

struct JSONSerializerVisitor : public IPropertyVisitor {

	JSONSerializerVisitor(nlohmann::json& json) : m_json(json) { }

	void operator()(std::string const& key, int& value, MetaData const& metaData = {}) override {
		m_json[key] = value;
	}
	void operator()(std::string const& key, float& value, MetaData const& metaData = {}) override {
		m_json[key] = value;
	}
	void operator()(std::string const& key, unsigned int& value, MetaData const& metaData = {}) override {
		m_json[key] = value;
	}
	void operator()(std::string const& key, std::string& value, MetaData const& metaData = {}) override {
		m_json[key] = value;
	}

protected:
	nlohmann::json& m_json;

	std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key, MetaData const& metaData) override;
	std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, MetaData const& metaData) override;
	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData) override;

	void handleFactoryInternal(nlohmann::json& json, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData);
};

struct JSONSerializerCollectionVisitor : public JSONSerializerVisitor {

	JSONSerializerCollectionVisitor(nlohmann::json& json) : JSONSerializerVisitor(json) { }

	void operator()(std::string const& key, int& value, MetaData const& metaData = {}) override {
		m_json.push_back(value);
	}
	void operator()(std::string const& key, float& value, MetaData const& metaData = {}) override {
		m_json.push_back(value);
	}
	void operator()(std::string const& key, unsigned int& value, MetaData const& metaData = {}) override {
		m_json.push_back(value);
	}
	void operator()(std::string const& key, std::string& value, MetaData const& metaData = {}) override {
		m_json.push_back(value);
	}

protected:
	std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key, MetaData const& metaData) override;
	std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, MetaData const& metaData) override;

	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData) override;

};


struct JSONDeserializerVisitor : public IPropertyVisitor {

	nlohmann::json& m_json;

	std::vector<nlohmann::json*> m_jsonStack; 

	JSONDeserializerVisitor(nlohmann::json& json) : m_json(json) { m_jsonStack.push_back(&m_json); }

	void operator()(std::string const& key, int& value, MetaData const& metaData = {}) override {
		if (!m_json.contains(key)) return;
		m_json[key].get_to(value);
	}
	void operator()(std::string const& key, float& value, MetaData const& metaData = {}) override {
		if (!m_json.contains(key)) return;
		m_json[key].get_to(value);
	}
	void operator()(std::string const& key, unsigned int& value, MetaData const& metaData = {}) override {
		if (!m_json.contains(key)) return;
		m_json[key].get_to(value);
	}
	void operator()(std::string const& key, std::string& value, MetaData const& metaData = {}) override {
		if (!m_json.contains(key)) return;
		m_json[key].get_to(value);
	}


protected:
	std::unique_ptr<IPropertyVisitor> childVisitor(std::string const& key, MetaData const& metaData) override {
		return nullptr;
	}
	std::unique_ptr<IPropertyVisitor> collectionVisitor(std::string const& key, MetaData const& metaData) override {
		return nullptr;
	}

	void handleFactory(std::string const& key, IFactoryElement*& pValue, IFactory& factory, MetaData const& metaData) override {

	}
};

